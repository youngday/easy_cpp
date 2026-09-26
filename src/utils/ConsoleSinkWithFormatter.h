#pragma once

#include "quill/backend/PatternFormatter.h"
#include "quill/core/Attributes.h"
#include "quill/core/Common.h"
#include "quill/core/Filesystem.h"
#include "quill/sinks/ConsoleSink.h"

#include <string>
#include <string_view>

class ConsoleSinkWithFormatter : public quill::ConsoleSink
{
public:
  ConsoleSinkWithFormatter(quill::PatternFormatterOptions const& pattern_formater_options,
                           bool enable_colours = true, std::string const& stream = "stdout")
    : quill::ConsoleSink(make_config(enable_colours, stream)), _formatter(pattern_formater_options)
  {
  }

  void write_log(quill::MacroMetadata const* log_metadata, uint64_t log_timestamp,
                 std::string_view thread_id, std::string_view thread_name,
                 std::string const& process_id, std::string_view logger_name, quill::LogLevel log_level,
                 std::string_view log_level_description, std::string_view log_level_short_code,
                 std::vector<std::pair<std::string, std::string>> const* named_args,
                 std::string_view log_message, std::string_view) override
  {
    // quill v12: PatternFormatter::format 末尾新增了 mdc 参数
    std::string_view const formatted_log_statement =
      _formatter.format(log_timestamp, thread_id, thread_name, process_id, logger_name, log_level_description,
                        log_level_short_code, *log_metadata, named_args, log_message, std::string_view{});

    quill::ConsoleSink::write_log(log_metadata, log_timestamp, thread_id, thread_name, process_id,
                                  logger_name, log_level, log_level_description, log_level_short_code,
                                  named_args, log_message, formatted_log_statement);
  }

private:
  // quill v12: ConsoleSink 改为通过 ConsoleSinkConfig 配置颜色模式与输出流
  static quill::ConsoleSinkConfig make_config(bool enable_colours, std::string const& stream)
  {
    quill::ConsoleSinkConfig config;
    config.set_stream(stream);
    config.set_colour_mode(enable_colours ? quill::ConsoleSinkConfig::ColourMode::Always
                                          : quill::ConsoleSinkConfig::ColourMode::Never);
    return config;
  }

  quill::PatternFormatter _formatter;
};
