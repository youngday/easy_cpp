#!/bin/sh
# check_versions.sh —— 查询 xmake.lua 里的依赖在 xmake-repo 中的最新版本
#
# 用法:
#   sh scripts/check_versions.sh                 # 自动解析 xmake.lua 中的依赖
#   sh scripts/check_versions.sh fmt zlib openssl# 只查指定的包
#   UPDATE=1 sh scripts/check_versions.sh        # 先执行 xrepo update-repo，再查
#   OUT=/tmp/ver.txt sh scripts/check_versions.sh  # 结果写入指定文件（默认 <项目根>/versions.txt）
#
# 原理:
#   `xrepo info <pkg>` 会让 xmake 自己解析出该包在仓库中的最新版本；
#   其输出带 ANSI 颜色码，且日志走 stderr，所以先 2>&1 合并、去色，
#   再定位到目标包的块（require(<pkg>): ... -> version: ...）取版本。
#
# 注意:
#   - 结果取决于当前本地 xmake-repo 快照，想拿最新快照请先 UPDATE=1。
#   - 本脚本只读，不改动任何文件。

set -eu

root_dir=$(cd "$(dirname "$0")/.." && pwd)
xmake_file="$root_dir/xmake.lua"

if [ "${UPDATE:-0}" = "1" ]; then
    xrepo update-repo
fi

# ---- 1. 收集 "包名 [版本]"(未指定参数时，从 xmake.lua 解析) ----
if [ "$#" -gt 0 ]; then
    specs=$(printf '%s\n' "$@")
else
    specs=$(awk '
        /^local[ \t]+libs[ \t]*=/ { inlibs = 1 }
        inlibs && /^}/            { inlibs = 0 }
        inlibs                    { print }
        /add_requires\(/          { inreq = 1 }
        inreq                     { print }
        inreq && /\)/             { inreq = 0 }
    ' "$xmake_file" | grep -o '"[^"]*"' | tr -d '"' || true)
fi

# 只保留 "包名 [版本]" 形式，过滤掉文件路径 / configs 值 / 别名等
specs=$(printf '%s\n' "$specs" \
    | grep -E '^[A-Za-z0-9][A-Za-z0-9_+.-]*( +[A-Za-z0-9.+_-]+)?$' \
    | sort -u || true)

# ---- 2. 逐包查询最新版，同时输出到终端与文件 ----
out_file=${OUT:-$root_dir/versions.txt}

{
printf '%-28s %-16s %s\n' 'PACKAGE' 'LATEST' 'PINNED'
printf '%-28s %-16s %s\n' '----------------------------' '---------------' '------'

printf '%s\n' "$specs" | while read -r spec; do
    [ -n "$spec" ] || continue
    name=${spec%% *}
    pin=${spec#* }
    [ "$pin" = "$spec" ] && pin='-'

    ver=$(xrepo info "$name" 2>&1 \
        | sed 's/\x1b\[[0-9;]*m//g' \
        | grep -m1 -A3 -F "require($name):" \
        | grep -m1 -- '-> version:' \
        | sed 's/.*-> version:[[:space:]]*//' || true)
    [ -n "$ver" ] || ver='(not found)'

    printf '%-28s %-16s %s\n' "$name" "$ver" "$pin"
done
} | tee "$out_file"

printf '\n结果已写入: %s\n' "$out_file"
