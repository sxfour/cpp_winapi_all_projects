using OpcUaHelper;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

using System.Text.Json;
using System.IO;

namespace wpf_tsrv
{
    /// <summary>
    /// Логика взаимодействия для Window1.xaml
    /// </summary>
    public partial class Window1 : Window
    {
        public class Data
        {
            public string ServerDataTSR024 { get; set; }
            public string ServerDataTSR023 { get; set; }
        }

        public Window1()
        {
            string text = File.ReadAllText("params.json");
            var data = JsonSerializer.Deserialize<Data>(text);

            string ServerTSR024 = data.ServerDataTSR024.ToString();
            string ServerTSR023 = data.ServerDataTSR023.ToString();

            InitializeComponent();
            InitializeOPC(ServerTSR024, ServerTSR023);
        }

        private void InitializeOPC(string ServerTSR024, string ServerTSR023)
        {
            var tags_tsrv024_33km = new[]
            {
                "ns=1;s=VZLJOT(Пикет №1).TCP-024M.Measure.Pressure.P1",
            };

            var tags_tsr023_33km = new[]
            {
                "ns=1;s=VZLJOT(33км).TCP-022.Measure.Pressure.Channel.P2",
                "ns=1;s=VZLJOT(33км).TCP-022.Measure.Pressure.Channel.P3",
            };

            Task.Run(async () =>
            {
                var values_tsr024_33km = new List<double>();
                var values_tsr023_33km = new List<double>();

                while (true)
                {
                    try
                    {
                        OpcUaClient m_OpcUaClient = new OpcUaClient();
                        //m_OpcUaClient.UserIdentity = new UserIdentity(new AnonymousIdentityToken());

                        await m_OpcUaClient.ConnectServer(ServerTSR024);

                        foreach (string i in tags_tsrv024_33km)
                        {
                            double data;

                            data = Double.Parse(m_OpcUaClient.ReadNode(i).ToString());

                            values_tsr024_33km.Add(Math.Round(data, 2));
                        }

                        m_OpcUaClient.Disconnect();
                    }
                    catch
                    {
                        continue;
                    }

                    try
                    {
                        OpcUaClient m_OpcUaClient = new OpcUaClient();
                        // m_OpcUaClient.UserIdentity = new UserIdentity(new AnonymousIdentityToken());

                        await m_OpcUaClient.ConnectServer(ServerTSR023);

                        foreach (string i in tags_tsr023_33km)
                        {
                            double data;

                            data = Double.Parse(m_OpcUaClient.ReadNode(i).ToString());

                            values_tsr023_33km.Add(Math.Round(data, 2));
                        }

                        m_OpcUaClient.Disconnect();
                    }
                    catch
                    {
                        continue;
                    }

                    // TSR-024
                    Dispatcher.Invoke(new Action(() => label_pressure_dy800.Content = values_tsr024_33km.ElementAt(0) + "кгс/см2"));

                    values_tsr024_33km.Clear();

                    // TSR-023
                    Dispatcher.Invoke(new Action(() => label_pressure_dy600.Content = values_tsr023_33km.ElementAt(0) + "кгс/см2"));
                    Dispatcher.Invoke(new Action(() => label_pressure_dy500.Content = values_tsr023_33km.ElementAt(1) + "кгс/см2"));

                    values_tsr023_33km.Clear();

                    Thread.Sleep(5000);
                }
            });
        }
    }
}
