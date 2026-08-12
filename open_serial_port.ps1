# Configure and Open Port
$port = New-Object System.IO.Ports.SerialPort COM3, 115200, None, 8, One
$port.Open()

Write-Host "Listening to COM3... Press 'Q' to quit and release the port safely." -ForegroundColor Green

while ($true) {
    # Check if a key was pressed to exit cleanly
    if ([console]::KeyAvailable) {
        $key = [console]::ReadKey($true)
        if ($key.KeyChar -eq 'q' -or $key.KeyChar -eq 'Q') { break }
    }

    # Safely read line data
    try { 
        if ($port.BytesToRead -gt 0) {
            Write-Host $port.ReadLine() 
        }
    } 
    catch { break }
    
    Start-Sleep -Milliseconds 50 # Prevents CPU spiking
}

# CRITICAL FIX: Explicitly hand the port back to Windows
$port.Close()
$port.Dispose()
Write-Host "Port closed successfully. You can now open PuTTY." -ForegroundColor Yellow
