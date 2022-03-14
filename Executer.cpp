#include "Executer.hpp"
extern std::vector<std::exception> ex_log;


void Executer::Consumer()
{
	while (true)
	{
		try
		{
			while (ConsumerEnabled)
			{
				while (RawDataQueue.ReadAvailable() == 0)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}

				auto currentPackage = RawDataQueue.Dequeue();

				//HEADER BYTE 1
				if (currentPackage->Data.Dequeue() == 0xFF)
				{
					//HEADER BYTE 2
					if (currentPackage->Data.Dequeue() == 0xFF)
					{
						//HEADER BYTE 3
						if (currentPackage->Data.Dequeue() == 0xFF)
						{
							//HEADER BYTE 4
							if (currentPackage->Data.Dequeue() == 0xFF)
							{
								//NON HEADER BYTE
								if (currentPackage->Data.Peek() != 0xFF)
								{
								HeaderFound:
									{
										auto payloadSize = Parse4ConsecutiveByte(&(currentPackage->Data));
										auto messageType = (RxMessageBase::MessageType)(Parse4ConsecutiveByte(&(currentPackage->Data)));
										auto senderUserName = ParseNConsecutiveByte(&(currentPackage->Data), 32);
										auto message = RxMessageBase::CreateInstance(messageType);
										message->userName = senderUserName;
										message->m_endpoint = currentPackage->m_endpoint;

										for (int i{}; i < payloadSize; ++i)
										{
											message->payload.push_back(currentPackage->Data.Dequeue());
										}

										auto checksum = Parse4ConsecutiveByte(&(currentPackage->Data));

										if (ChecksumValidator(message->payload, checksum))
										{
											message->payloadSize = payloadSize;
											message->Parse();
											message->DoJob(&TransmitMessageQueue);
											//ReceivedMessageLogs.Enqueue(message);
										}

										else
										{
											delete message;
											throw std::exception("Wrong checksum!");
										}
									}
								}
							}
						}
					}
				}

			}

			//Consumer is not active
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		catch (const std::exception& ex)
		{

		}
	}
}

void Executer::Producer()
{
	std::array<unsigned char, 1024> recv_buf;
	boost::asio::ip::udp::endpoint sender_endpoint;

	while (true)
	{
		try
		{
			while (ProducerEnabled)
			{
				size_t len = socket.receive_from(
					boost::asio::buffer(recv_buf), sender_endpoint);	//synchronous function

				std::cout << "Data: " << len << "byte" << " Sender: " << sender_endpoint.address() << "/" << sender_endpoint.port() << "\n";

				//Create new endpoint
				auto end_point = new boost::asio::ip::udp::endpoint();
				*end_point = sender_endpoint;

				auto msg_pack = new MessagePack(end_point);
				for (int i{}; i < len; ++i)
				{
					msg_pack->Data.Enqueue(recv_buf[i]);
				}

				RawDataQueue.Enqueue(msg_pack);
			}

			//Producer is not activated
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		catch (const std::exception& ex)
		{
			
		}

	}
}

void Executer::TransmitData()
{
	while (true)
	{
		try
		{
			while (TransmitterEnabled)
			{
				while (TransmitMessageQueue.ReadAvailable() == 0)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}

				TransmitMessageQueue.Dequeue()->SendMsg(socket);
			}

			//Consumer is not active
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}


		catch (const std::exception& ex)
		{

		}
	}
}