# List all COM ports
[System.IO.Ports.SerialPort]::GetPortNames()

# Or find the ST-LINK VCP specifically
Get-PnpDevice | Where-Object { $_.FriendlyName -like "*STLink*" -or $_.FriendlyName -like "*ST-LINK*" } |
    Select-Object FriendlyName, Status