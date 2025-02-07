using System;
using System.Collections.Generic;
using System.Data;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

using Npgsql;

namespace wpf_tsrv
{
    /// <summary>
    /// Логика взаимодействия для Window2.xaml
    /// </summary>
    /// 

    public partial class Window2 : Window
    {
        public Window2()
        {
            InitializeComponent();
            InitDatabase();
        }

        private void InitDatabase()
        {
            // Правильное отображение времени
            AppContext.SetSwitch("Npgsql.EnableLegacyTimestampBehavior", true);
            AppContext.SetSwitch("Npgsql.DisableDateTimeInfinityConversions", true);
            
            try
            {
                var cs = "Host=192.168.0.126;Port=6889;Username=tsr_Archive;Password=teplos2006@#;Database=tsrData;Timeout=1";

                NpgsqlConnection con = new NpgsqlConnection(cs);

                string date_yest = DateTime.Today.AddDays(-1).ToString("yyyy-MM-dd");
                string date_now = DateTime.Now.ToString("yyyy-MM-dd");
                string date_req = string.Format("SELECT outdoor_temp, TO_CHAR(created_at, 'HH24:MI:SS | dd.MM.yyyy')  FROM tsrvalues WHERE created_at BETWEEN '{0} 23:59:00' AND '{1} 23:59:00';", date_yest, date_now);
                
                con.Open();
                
                var sql = date_req;

                NpgsqlCommand cmd = new NpgsqlCommand(sql, con);

                var dataReader = cmd.ExecuteReader();

                DataTable dt = new DataTable();
                dt.Load(dataReader);
                con.Close();

                dataGridView1.ItemsSource = dt.DefaultView;

            } catch
            {
                System.Windows.MessageBox.Show("Упс, отчёт не может быть сформирован.\n", "Ошибка при подключении к базе данных", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void buttonGenerate_Click(object sender, RoutedEventArgs e)
        {
            // Правильное отображение времени
            AppContext.SetSwitch("Npgsql.EnableLegacyTimestampBehavior", true);
            AppContext.SetSwitch("Npgsql.DisableDateTimeInfinityConversions", true);
            try
            {
                var cs = "Host=192.168.0.126;Port=6889;Username=tsr_Archive;Password=teplos2006@#;Database=tsrData;Timeout=1";

                NpgsqlConnection con = new NpgsqlConnection(cs);

                string date_yest = calendar.SelectedDate.Value.ToString("yyyy-MM-dd");
                string date_now = calendar.SelectedDate.Value.ToString("yyyy-MM-dd");
                string date_req = string.Format("SELECT outdoor_temp, TO_CHAR(created_at, 'HH24:MI:SS | dd.MM.yyyy')  FROM tsrvalues WHERE created_at BETWEEN '{0} 00:00:00' AND '{1} 23:59:59';", date_yest, date_now);
                
                con.Open();
                
                var sql = date_req;

                NpgsqlCommand cmd = new NpgsqlCommand(sql, con);

                var dataReader = cmd.ExecuteReader();

                DataTable dt = new DataTable();
                dt.Load(dataReader);
                con.Close();

                dataGridView1.ItemsSource = dt.DefaultView;
            } catch
            {
                System.Windows.MessageBox.Show("Упс, отчёт не может быть сформирован.\n", "Ошибка при подключении к базе данных", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            
        }
    }
}
