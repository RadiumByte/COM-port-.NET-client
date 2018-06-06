#include "stdafx.h"

using namespace System;
using namespace System::IO::Ports;
using namespace std;

void Open_Connetion(SerialPort ^port, String ^COM_name, int BaudRate)
{
	try
	{
		cout << endl;
		if (port->IsOpen)
		{
			port->Close();
			cout << "Closing port, because it was already open!" << endl;
		}
		else
		{
			port->ReadTimeout = 1000;
			port->PortName = COM_name;
			port->BaudRate = BaudRate;
			port->DataBits = 8;
			port->Parity = Parity::None;
			port->StopBits = StopBits::One;

			port->Open();
			cout << "Port opened!" << endl;
		}
	}
	catch (System::Exception ^e)
	{
		Console::WriteLine("Error was currupted: " + e->Message);
	}
}

void main()
{
	{
		SerialPort ^port = gcnew SerialPort();

		cout << "COM RS-232 Control" << endl;
		cout << "Version 1.0" << endl << endl;

		bool exit_flag = false;
		bool exit_flag_browser = false;
		bool is_empty = false;
		int global_command = 0;
		int browser_command = 0;

		while (!exit_flag)
		{
		begin_of_program:
			cout << endl;
			cout << "Select the command from list below: " << endl;
			cout << "(1) Browse all COM-devices and connect" << endl;
			cout << "(2) Connect directly" << endl;
			cout << "(3) Send a command" << endl;
			cout << "(4) Close current connection" << endl << endl;
			cout << "(5) Exit from program" << endl << endl;

			cout << "Enter the command's number: ";
			cin >> global_command;
			cout << endl;

			switch (global_command)
			{
				///////////////// Начало - выбор COM-устройства ///////////////////////////////
			case 1:
			{
			begin_of_browser:
				cout << "List of avaliable devices: " << endl;
				cli::array<System::String ^> ^all_ports = SerialPort::GetPortNames();
				if (all_ports->Length == 0)
				{
					cout << "List is empty" << endl;
					is_empty = true;
				}
				else
				{
					is_empty = false;
					for each (String ^ current in all_ports)
						Console::WriteLine(current);
					cout << endl;
				}
				while (!exit_flag_browser)
				{
					cout << endl;
					cout << "(1) Connect to device" << endl;
					cout << "(2) Refresh" << endl << endl;
					cout << "(3) <- Back" << endl << endl;

					cout << "Enter the command's number: ";
					cin >> browser_command;
					cout << endl;

					switch (browser_command)
					{
					case 1:
					{
						if (is_empty)
						{
							cout << "List is empty - connection failure" << endl << endl;
							goto begin_of_browser;
						}
						else
						{
							cout << "Enter COM-port name: ";
							String ^ COM_name = Console::ReadLine();
							Open_Connetion(port, COM_name, 9600);
						}

					}
					case 2:
					{
						cout << "Refreshing...." << endl << endl;
						goto begin_of_browser;
					}
					case 3:
						goto begin_of_program;
					default:
						cout << "Invalid command. Print another one." << endl;
						break;
					}
					break;
				}
			} ///////////////// Конец - Выбор COM-устройства ///////////////////////
			  ///////////////// Начало - прямое подключение ////////////////////////
			case 2:
			{
				cout << "Direct connection. Enter the COM-device: ";
				String ^COM_name = Console::ReadLine();
				Open_Connetion(port, COM_name, 19200);
				break;
			} ///////////////// Конец - прямое подключение ////////////////////////
			  ///////////////// Начало - команды ////////////////////////
			case 3:
			{
				cout << "Command mode. Print a command to send. To exit - print 'EXIT': " << endl;
				if (!port->IsOpen)
					cout << "Port is closed. Commanding is unalvaliable." << endl;
				else
				{
					String ^current_command = Console::ReadLine();
					while (current_command != "EXIT")
					{
						port->WriteLine(current_command);
						current_command = Console::ReadLine();
					}
				}
				break;
			} ///////////////// Конец - команды ////////////////////////
			  ///////////////// Начало - закрытие порта ////////////////////////
			case 4:
			{
				if (port->IsOpen)
					port->Close();
				cout << "Port is closed" << endl;
				break;
			} ///////////////// Конец - закрытие порта ////////////////////////
			case 5:
			{
				exit_flag = true;
				break;
			}
			default:
				cout << "Invalid command. Print another one." << endl;
				break;
			}
		}
	}
	system("pause");
}
