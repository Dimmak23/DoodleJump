Write-Host "size: ";
(Get-ChildItem -Path ../. -Recurse | Measure-Object -Property Length -Sum).Sum / 1GB;
Write-Host " (GB)."