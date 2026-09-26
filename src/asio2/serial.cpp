#include <asio2/serial_port/serial_port.hpp>
#include <iostream>
#include <filesystem>

int main()
{

	//std::string_view device = "COM1"; // for windows
	std::string_view device = "/dev/ttyS0"; // for linux
	std::string_view baud_rate = "9600";
	
	// Check if serial device exists and is accessible
	if (!std::filesystem::exists(device)) {
		std::cout << "Error: Serial device " << device << " does not exist" << std::endl;
		std::cout << "Available serial devices:" << std::endl;
		system("ls -la /dev/tty* 2>/dev/null | grep -E 'ttyS|ttyUSB|ttyACM'");
		return 1;
	}

	asio2::serial_port sp;

	sp.bind_init([&]()
	{
		// Set other serial port parameters at here
		sp.set_option(asio::serial_port::flow_control(asio::serial_port::flow_control::type::none));
		sp.set_option(asio::serial_port::parity(asio::serial_port::parity::type::none));
		sp.set_option(asio::serial_port::stop_bits(asio::serial_port::stop_bits::type::one));
		sp.set_option(asio::serial_port::character_size(8));

	}).bind_recv([&](std::string_view data)
	{
		printf("recv : %zu %.*s\n", data.size(), (int)data.size(), data.data());

		sp.async_send(data);

	}).bind_start([&]()
	{
		printf("start : %d %s\n", asio2::last_error_val(), asio2::last_error_msg().c_str());
	}).bind_stop([&]()
	{
		printf("stop : %d %s\n", asio2::last_error_val(), asio2::last_error_msg().c_str());
	});

	//sp.start(device, baud_rate);
	if (!sp.start(device, baud_rate, '\n')) {
		std::cout << "Failed to start serial port. Check permissions or try:" << std::endl;
		std::cout << "sudo chmod 666 " << device << std::endl;
		std::cout << "Or add your user to dialout group: sudo usermod -a -G dialout $USER" << std::endl;
		return 1;
	}
	//sp.start(device, baud_rate, "\r\n");
	//sp.start(device, baud_rate, match_role);
	//sp.start(device, baud_rate, asio::transfer_at_least(1));
	//sp.start(device, baud_rate, asio::transfer_exactly(10));

	sp.async_send("abc0123456789xyz\n", [](std::size_t bytes_sent)
	{
		printf("send : %zu %d %s\n", bytes_sent,
			asio2::last_error_val(), asio2::last_error_msg().c_str());
	});

	std::cout << "Serial port running. Press Enter to exit..." << std::endl;
	while (std::getchar() != '\n');

	return 0;
}
