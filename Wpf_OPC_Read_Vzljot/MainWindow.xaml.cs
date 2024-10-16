using System;
using System.Collections.Generic;
using System.Diagnostics;
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

using Opc.Ua;
using OpcUaHelper;
using wpf_tsrv;
using System.Text.Json;
using System.IO;


namespace wpf_opcua_tsr
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public class Data
        {
            public string ServerDataTSR024 { get; set; }
            public string ServerDataTSR023 { get; set; }
            public string OpenNewTab { get; set; }
        }


        public MainWindow()
        {
            string text = File.ReadAllText("params.json");
            var data = JsonSerializer.Deserialize<Data>(text);
            string ServerTSR024 = data.ServerDataTSR024.ToString();
            string ServerTSR023 = data.ServerDataTSR023.ToString();
            bool OpenNewWindow = Convert.ToBoolean(data.OpenNewTab.ToString());

            // Инициализация
            InitializeComponent();
            InitializeCurrentTime();
            InitializeDataFromOPC(ServerTSR024, ServerTSR023);
            InitializeNewWindow(OpenNewWindow);
        }


        private void InitializeDataFromOPC(string ServerTSR024, string ServerTSR023)
        {
            // long totalMemory = GC.GetTotalMemory(false);

            // Теги (Задаются в Protocol Master)
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

            // Запуск опроса в потоке
            Task.Run(async () =>
            {
                var values_tsr024 = new List<double>();
                var  values_tsr023 = new List<double>();

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

                            // Преобразование в число с точкой + округление 
                            data = Double.Parse(m_OpcUaClient.ReadNode(i).ToString());

                            values_tsr024.Add(Math.Round(data, 2));
                        }

                        m_OpcUaClient.Disconnect();

                        Dispatcher.Invoke(new Action(() => label_status.Background = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromRgb(86, 255, 60))));
                        Dispatcher.Invoke(new Action(() => label_status.Text = "Опрос: запущен..."));
                        Dispatcher.Invoke(new Action(() => loop.Opacity = 100));
                    }
                    catch
                    {
                        Dispatcher.Invoke(new Action(() => label_status.Background = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromRgb(255, 90, 90))));
                        Dispatcher.Invoke(new Action(() => label_status.Text = "Опрос: ошибка..."));
                        Dispatcher.Invoke(new Action(() => splash_load.Opacity = 0));
                        Dispatcher.Invoke(new Action(() => loop.Opacity = 0));

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
                        Dispatcher.Invoke(new Action(() => loop.Opacity = 100));
                    }
                    catch
                    {
                        Dispatcher.Invoke(new Action(() => label_status.Background = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Color.FromRgb(255, 90, 90))));
                        Dispatcher.Invoke(new Action(() => label_status.Text = "Опрос: ошибка..."));
                        Dispatcher.Invoke(new Action(() => splash_load.Opacity = 0));
                        Dispatcher.Invoke(new Action(() => loop.Opacity = 0));

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

                    Dispatcher.Invoke(new Action(() => splash_load.Opacity = 0));

                    // GC.Collect(0, GCCollectionMode.Forced);
                    // GC.WaitForPendingFinalizers();

                    Thread.Sleep(5000);
                }
            });
        }

        // Счётчик времени (локальное время пк)
        private void InitializeCurrentTime()
        {
            Task.Run(async () =>
            {
                while (true)
                {
                    Dispatcher.Invoke(new Action(() => label_time.Content = DateTime.Now.ToString("dd.MM.yyyy HH:mm:ss tt")));

                    int time = DateTime.Now.Hour;

                    if (time == 0 || time == 1)
                    {
                        Dispatcher.Invoke(new Action(() => label_alert_reload.Opacity = 100));
                    } 
                    else
                    {
                        Dispatcher.Invoke(new Action(() => label_alert_reload.Opacity = 0));
                    }

                    Thread.Sleep(1000);
                }
            });
        }

        // Автозапуск второго окна
        private void InitializeNewWindow(bool OpenNewWindow)
        {
            if (OpenNewWindow)
            {
                Window1 AutoWindow = new Window1();
                AutoWindow.Show();
            }
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

        private void ItemClickInfo(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Опрос ОПС серверов\ndev: Egor Levashov\nlang: C#, Версия 0.1.6\n2023 - 2024, license gpl3.", "Информация о программе", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void ItemClickLicense(object sender, RoutedEventArgs e)
        {
            Process.Start(".\\LICENSE.txt");
        }

        private void ItemClickConfig(object sender, RoutedEventArgs e)
        {
            Process.Start(".\\params.json");
        }

        private void ItemClickTSRV024Python(object sender, RoutedEventArgs e)
        {
            Process.Start(".\\tsrv024m.exe");
        }

        private void ItemClickTSRV023Python(object sender, RoutedEventArgs e)
        {
            Process.Start(".\\tsrv023.exe");
        }

        private void MediaElement_MediaEnded(object sender, RoutedEventArgs e)
        {
            loop.Position = TimeSpan.FromMilliseconds(1);
        }

        private void MediaElement_MediaEnded_back(object sender, RoutedEventArgs e)
        {
            background.Position = TimeSpan.FromMilliseconds(1);
        }
    }
}
