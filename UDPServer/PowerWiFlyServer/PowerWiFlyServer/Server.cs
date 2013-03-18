using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace PowerWiFlyServer
{
    class Server
    {
        byte[] data;            //data buffer
        IPEndPoint ipep;        //receiving information
        UdpClient newsock;      //UDP socket
        IPEndPoint sender;      //sender information

        public Server(int port)
        {
            data = new byte[1024];
            ipep = new IPEndPoint(IPAddress.Any, port);
            newsock = new UdpClient(ipep);
            sender = new IPEndPoint(IPAddress.Any, 0);
        }

        public int ListeAndRespond(){
            data = newsock.Receive(ref sender);
            Console.WriteLine("Message received from {0}:", sender.ToString());

            Console.WriteLine(Encoding.ASCII.GetString(data, 0, data.Length));
            newsock.Send(data, data.Length, sender);
            string welcome = "Welcome to my test server";
            data = Encoding.ASCII.GetBytes(welcome);
            newsock.Send(data, data.Length, sender);
            return 0;
        }

        /*
        private string color;

        public Server(string color)
        {
            this.color = color;
        }

        public string Describe()
        {
            return "This car is " + Color;
        }

        public string Color
        {
            get { return color; }
            set { color = value; }
        }
         * */
    }
}
