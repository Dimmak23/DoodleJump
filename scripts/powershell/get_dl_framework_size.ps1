Write-Host "size: "; (Get-ChildItem -Path dragon_lake_framework -Recurse | Measure-Object -Property Length -Sum).Sum / 1MB; Write-Host " (MB)."