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
            try
            {
                string sUri = "http://www.google.com.ar/directorio/subdirectorio?variable1=valor1;variable2=valor2";

                System.Uri myUri = new System.Uri(sUri);
                
                HttpWebRequest HttpWRequest = (HttpWebRequest)WebRequest.Create(myUri);

                string sQuery;
                sQuery = "<?xml version='1.0'?>" +
                   "<a:propertyupdate xmlns:a='DAV:' " +
                   "xmlns:m='urn:schemas:mailheader:'>" +
                   "<a:set><a:prop>" +
                   "<m:subject>" + "ModifiedSubject" + "</m:subject>" +
                   "</a:prop></a:set>" +
                   "</a:propertyupdate>";

                //NetworkCredential myCred = new NetworkCredential(@"DomainName\UserName", "Password");
                //CredentialCache myCredentialCache = new CredentialCache();
                //myCredentialCache.Add(myUri, "Basic", myCred);
                //HttpWRequest.Credentials = myCredentialCache;

                //Uncomment the following statement and comment the previous 4 statements if you 
                //use Integrated Windows authentication
                //httpWRequest.Credentials = CredentialCache.DefaultCredentials

                //Note In Basic type authentication, the username and the password are sent as base64-encoded text, which is 
                //easily decoded. Microsoft recommends that you use Basic over SSL to help protect the username and the password.

                // Set Headers
                HttpWRequest.KeepAlive = false;
                //HttpWRequest.Headers.Set("Pragma", "no-cache");
                //HttpWRequest.Headers.Set("Translate", "f");
                HttpWRequest.ContentType = "text/xml";
                HttpWRequest.ContentLength = sQuery.Length;

                Console.WriteLine(HttpWRequest.Headers.Count);


                //set the request timeout to 5 min.
                HttpWRequest.Timeout = 300000;
                // set the request method
                HttpWRequest.Method = "PROPPATCH";

                // You must store the data in a byte array
                byte[] ByteQuery = System.Text.Encoding.ASCII.GetBytes(sQuery);
                HttpWRequest.ContentLength = ByteQuery.Length;
                Stream QueryStream = HttpWRequest.GetRequestStream();
                // Write the data to be posted to the Request Stream
                QueryStream.Write(ByteQuery, 0, ByteQuery.Length);
                QueryStream.Close();

                // Send Request and Get Response
                HttpWebResponse HttpWResponse = (HttpWebResponse)HttpWRequest.GetResponse();

                // Get the Status code
                int iStatCode = (int)HttpWResponse.StatusCode;
                string sStatus = iStatCode.ToString();
                Console.WriteLine("Status Code: {0}", sStatus);
                // Get the request headers
                string sReqHeaders = HttpWRequest.Headers.ToString();
                Console.WriteLine(sReqHeaders);

                // Read the Response Stream
                Stream strm = HttpWResponse.GetResponseStream();
                StreamReader sr = new StreamReader(strm);
                string sText = sr.ReadToEnd();
                Console.WriteLine("Response: {0}", sText);

                // Close Stream
                strm.Close();

                // Clean Up
                //myCred = null;
                //myCredentialCache = null;
                HttpWRequest = null;
                HttpWResponse = null;
                QueryStream = null;
                strm = null;
                sr = null;
            }
            catch (Exception e)
            {
                Console.WriteLine("{0} Exception caught.", e);
            }


            //---------------------------------------------------------------------


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
