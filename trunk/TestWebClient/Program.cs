using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.IO;

namespace TestWebClient
{
    class Program
    {
        public static void Main(string[] args)
        {
            //if (args == null || args.Length == 0)
            //{
            //    throw new ApplicationException("Specify the URI of the resource to retrieve.");
            //}
            //string uri = args[0];


            string uri = "http://www.google.com";

            WebClient client = new WebClient();

            // Add a user agent header in case the 
            // requested URI contains a query.

            client.Headers.Add("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; .NET CLR 1.0.3705;)");

            Stream data = client.OpenRead(uri);
            Console.WriteLine(client.ResponseHeaders.Count);
            StreamReader reader = new StreamReader(data);
            string s = reader.ReadToEnd();
            Console.WriteLine(s);
            data.Close();
            reader.Close();
        }
    }
}
