using System;
using System.Collections.Generic;
using System.Text;
using System.Net.Http;
using System.Threading.Tasks;

namespace App1.Services
{
    public interface ITempService
    {
    }

    public class TempService : ITempService
    {
        private HttpClient client;

        public  TempService()
        {
            client = new HttpClient();
            Console.WriteLine( RefreshDataAsync().Result);
            Console.WriteLine( "TempService initialized...");
        }

        public async Task<string> RefreshDataAsync()
        {
            try
            {
                Uri uri = new Uri(string.Format("http://192.168.5.179", string.Empty));
                HttpResponseMessage response = await client.GetAsync(uri);
                if (response.IsSuccessStatusCode)
                {
                    string content = await response.Content.ReadAsStringAsync();
                    return content;
                    //                Items = JsonSerializer.Deserialize<List<TodoItem>>(content, serializerOptions);
                }
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            return null;
        }
    }
}
