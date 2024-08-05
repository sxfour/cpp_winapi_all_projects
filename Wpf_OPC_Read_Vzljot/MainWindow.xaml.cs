using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Text.Json;

using Opc.Ua;
using OpcUaHelper;
using wpf_tsrv;
using static System.Net.Mime.MediaTypeNames;


namespace wpf_opcua_tsr {

    public partial class MainWindow : Window
    {
        public class Data
        {
            public string ServerDataTSR024 { get; set; }
            public string ServerDataTSR023 { get; set; }
        }

        public MainWindow()
        {
            string text = File.ReadAllText("params.json");
            var data = JsonSerializer.Deserialize<Data>(text);

            string ServerTSR024 = data.ServerDataTSR024.ToString();
            string ServerTSR023 = data.ServerDataTSR023.ToString();

            InitializeCurrentTime();
            InitializeDataFromOPC(ServerTSR024, ServerTSR023);
        }


        private void InitializeDataFromOPC(string ServerTSR024, string ServerTSR023)
        {
            var tags_tsrv024 = new[]
            {
                "ns=1;s=VZLJOT(Пикет №1).TCP-024M.Measure.Temperature.T1",
                "ns=1;s=VZLJOT(Пикет №1).TCP-024M.Measure.Temperature.T2",
                "ns=1;s=VZLJOT(Пикет №1).TCP-024M.Measure.Temperature.T3",
                "ns=1;s=VZLJOT(Пикет №1).TCP-024M.Measure.Temperature.TXВ0",
                "ns=1;s=VZLJOT(Пикет №1).TCP-024M.Measure.FlowV.Q1",
                "ns=1;s=VZLJOT(Пикет №1).TCP-024M.Measure.FlowV.Q2",
                "ns=1;s=VZLJOT(Пикет №1).TCP-024M.Measure.FlowV.Q3",
                "ns=1;s=VZLJOT(Пикет №1).TCP-024M.Measure.Pressure.P1",
                "ns=1;s=VZLJOT(Пикет №1).TCP-024M.Measure.Pressure.P2",
                "ns=1;s=VZLJOT(Пикет №1).TCP-024M.Measure.Pressure.P3",
            };

            var tags_tsr023 = new[]
            {
                "ns=1;s=VZLJOT(33км).TCP-022.Measure.Pressure.Channel.P2",
                "ns=1;s=VZLJOT(33км).TCP-022.Measure.Pressure.Channel.P3",
                "ns=1;s=VZLJOT(33км).TCP-022.Measure.Pressure.Channel.P4",
                "ns=1;s=VZLJOT(33км).TCP-022.Measure.Temperature.Channel.T1",
                "ns=1;s=VZLJOT(33км).TCP-022.Measure.Temperature.Channel.T2",
                "ns=1;s=VZLJOT(33км).TCP-022.Measure.Temperature.Channel.T3",
                "ns=1;s=VZLJOT(33км).TCP-022.Measure.Temperature.Point.T5",
            };

            Task.Run(async () =>
            {
                var values_tsr024 = new List<double>();
                var values_tsr023 = new List<double>();

                while (true)
                {
                        try
                        {
                            OpcUaClient m_OpcUaClient = new OpcUaClient();
                            //m_OpcUaClient.UserIdentity = new UserIdentity(new AnonymousIdentityToken());

                            await m_OpcUaClient.ConnectServer(ServerTSR024);

                            foreach (string i in tags_tsrv024)
                            {
                                double data;

                                data = Double.Parse(m_OpcUaClient.ReadNode(i).ToString());

                                values_tsr024.Add(Math.Round(data, 2));
                            }

                            m_OpcUaClient.Disconnect();

                            Dispatcher.Invoke(new Action(() => label_status.Background = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromRgb(86, 255, 60))));
                            Dispatcher.Invoke(new Action(() => label_status.Text = "Опрос: запущен..."));
                        }
                        catch
                        {
                            Dispatcher.Invoke(new Action(() => label_status.Background = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromRgb(255, 90, 90))));
                            Dispatcher.Invoke(new Action(() => label_status.Text = "Опрос: ошибка..."));

                            continue;
                        }

                        try
                        {
                            OpcUaClient m_OpcUaClient = new OpcUaClient();
                           // m_OpcUaClient.UserIdentity = new UserIdentity(new AnonymousIdentityToken());

                            await m_OpcUaClient.ConnectServer(ServerTSR023);

                            foreach (string i in tags_tsr023)
                            {
                                double data;

                                data = Double.Parse(m_OpcUaClient.ReadNode(i).ToString());

                                values_tsr023.Add(Math.Round(data, 2));
                            }

                            m_OpcUaClient.Disconnect();

                            Dispatcher.Invoke(new Action(() => label_status.Background = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromRgb(86, 255, 60))));
                            Dispatcher.Invoke(new Action(() => label_status.Text = "Опрос: запущен..."));
                        }
                        catch
                        {
                            Dispatcher.Invoke(new Action(() => label_status.Background = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromRgb(255, 90, 90))));
                            Dispatcher.Invoke(new Action(() => label_status.Text = "Опрос: ошибка..."));

                            continue;
                        }

                    // TSR-024
                    Dispatcher.Invoke(new Action(() => label_temp_800.Content = values_tsr024.ElementAt(0) + "°C"));
                    Dispatcher.Invoke(new Action(() => label_pressure_800.Content = values_tsr024.ElementAt(7) + "кгс/см2"));
                    Dispatcher.Invoke(new Action(() => label_volume_800.Content = values_tsr024.ElementAt(4) + "м3/ч"));

                    Dispatcher.Invoke(new Action(() => label_temp_600.Content = values_tsr024.ElementAt(1) + "°C"));
                    Dispatcher.Invoke(new Action(() => label_pressure_600.Content = values_tsr024.ElementAt(8) + "кгс/см2"));
                    Dispatcher.Invoke(new Action(() => label_volume_600.Content = values_tsr024.ElementAt(5) + "м3/ч"));

                    Dispatcher.Invoke(new Action(() => label_temp_500.Content = values_tsr024.ElementAt(2) + "°C"));
                    Dispatcher.Invoke(new Action(() => label_pressure_500.Content = values_tsr024.ElementAt(9) + "кгс/см2"));
                    Dispatcher.Invoke(new Action(() => label_volume_500.Content = values_tsr024.ElementAt(6) + "м3/ч"));

                    Dispatcher.Invoke(new Action(() => label_temp_cold_water_value.Content = values_tsr024.ElementAt(3) + "°C"));

                    values_tsr024.Clear();

                    // TSR-023
                    Dispatcher.Invoke(new Action(() => label_pressure_dy600.Content = values_tsr023.ElementAt(0) + "кгс/см2"));
                    Dispatcher.Invoke(new Action(() => label_pressure_dy500.Content = values_tsr023.ElementAt(1) + "кгс/см2"));

                    Dispatcher.Invoke(new Action(() => label_temp_air_value.Content = values_tsr023.ElementAt(6) + "°C"));

                    Dispatcher.Invoke(new Action(() => label_reverse_value.Content = values_tsr023.ElementAt(2) + "кгс/см2"));

                    values_tsr023.Clear();

                    Thread.Sleep(5000);
                }
            });
        }

        private void InitializeCurrentTime()
        {
            Task.Run(async () =>
            {
                while (true)
                {
                    Dispatcher.Invoke(new Action(() => label_time.Content = DateTime.Now.ToString("dd.MM.yyyy HH:mm:ss tt")));
                    Thread.Sleep(1000);
                }
            });
        }

        private void ItemClick33km(object sender, RoutedEventArgs e)
        {
            Window1 bWin = new Window1();
            bWin.Owner = this;
            bWin.Show();
        }

        private void ItemClickExit(object sender, RoutedEventArgs e)
        {
            Environment.Exit(0);
        }

        private void ItemClickLicense(object sender, RoutedEventArgs e)
        {
            Process.Start(".\\LICENSE.txt");
        }
        
        private void ItemClickSettingsServer(object sender, RoutedEventArgs e)
        {
            Process.Start(".\\params.json");
        }

        private void ItemClickInfo(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Опрос ОПС серверов.\nРазработчик: Егор Левашов\nLang: C#\n2023-2024 Лицензия GPL3.", "Информация о программе", MessageBoxButton.OK, MessageBoxImage.Information);
        }
    }
}
