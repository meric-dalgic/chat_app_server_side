#include "Executer.hpp"

void Executer::Consumer()
{
	while (true)
	{
		while (ConsumerEnabled)
		{
			while (RawDataQueue.ReadAvailable() == 0)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}

			//std::cout << "In consumer available packages are " << RawDataQueue.ReadAvailable() << "\n";

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
									//Receive message contains information about username of sender
									message->userName = senderUserName;
									//Receive message contains information about endpoint of sender
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
}

void Executer::Producer()
{
	std::array<unsigned char, 1024> recv_buf;
	boost::asio::ip::udp::endpoint sender_endpoint;

	while (true)
	{
		while (ProducerEnabled)
		{
			size_t len = socket.receive_from(
				boost::asio::buffer(recv_buf), sender_endpoint);	//synchronous function

			std::cout << "Data: " << len << "byte" << " Sender: " << sender_endpoint.address() << "/" << sender_endpoint.port() << "\n";

			//TODO: remove
			//BEGIN: TEMPORARY INFORMATION REGION
			//std::cout << "Data received. Length = " << len << "\n";
			//for (int i{}; i < len; ++i)
			//	std::cout << (unsigned int)recv_buf[i] << " ";
			//std::cout << "\n";
			//std::cout << "sender address: " << sender_endpoint.address() << "\n";
			//std::cout << "sender port: " << sender_endpoint.port() << "\n";
			//END: TEMPORARY INFORMATION REGION

			//Create new endpoint
			auto end_point = new boost::asio::ip::udp::endpoint();
			*end_point = sender_endpoint;

			auto msg_pack = new MessagePack(end_point);
			for (int i{}; i < len; ++i)
			{
				msg_pack->Data.Enqueue(recv_buf[i]);
			}

			RawDataQueue.Enqueue(msg_pack);

			//TODO: remove
			//Deneme kodu
			//std::cout << "Available packs:" << RawDataQueue.ReadAvailable() << "\n";
			//std::cout << "Data length of first pack:" << RawDataQueue.Peek()->Data.ReadAvailable() << "\n";
		}

		//Producer is not activated
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	}
}

void Executer::TransmitData()
{
	while (true)
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
}