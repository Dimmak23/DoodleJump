# Set up parameters
param (
    [string]$jsonFilePath,
    [string]$config,
    [string]$generator
)

# Create a hashtable with the parameters
$data = @{
	Config    = $config
	Generator = $generator
}

# Convert the hashtable to JSON
$jsonData = $data | ConvertTo-Json -Depth 10

# Write the JSON data to a file
$jsonData | Out-File -FilePath $jsonFilePath -Encoding UTF8
