function open_ssh_keygen {
    $rand_string = ([System.Guid]::NewGuid()).ToString()
    Start-Process ssh-keygen -ArgumentList "-f C:/$rand_string" -WindowStyle maximized
}

open_ssh_keygen