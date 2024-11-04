# Set up parameters
param (
    [string]$rebuild,
    [string]$config,
    [string]$generator
)

if (($generator -or $config) -and $rebuild) {
    Write-Output "Wrong command. '-rebuild' can't be used with '-config' and/or '-generator'."
    exit 0
}

# Define the path to the JSON file
$jsonFilePath = "build_data.json"

# If NO JSON file exists: create default
if (-not (Test-Path $jsonFilePath)) {
    # ? Re-configuring and re-building all, cause there is no json file found before
    # ? All other parameters will be ignored

    Write-Output "Data about previous build didn't found. Going with 'Ninja' on 'Debug'..."

    # Re-config vendor, connector and game; also build vendor and connector
    & "sub_scripts/re_config_all.ps1" -jsonFilePath $jsonFilePath -config "Debug" -generator "Ninja"
    
    # Now we can re-build game itself
    & "sub_scripts/re_build_game.ps1" -config "Debug"

    exit 0
}

if($config -or $generator){

    # Check the value of $generator and convert it accordingly
    if ($generator -eq "Ninja") {
        Write-Output "Specified $generator as generator."
    }
    elseif ($generator -eq "MinGW") {
        $generator = "MinGW Makefiles"
        Write-Output "Specified Ninja as generator."
    }
    elseif ($generator -eq "VS") {
        $generator = "Visual Studio 17 2022"
    }
    else {
        Write-Output "Invalid generator specified: $generator. Using 'Ninja' as default..."
        $generator = "Ninja"
    }

    if (-not ($config -eq "Debug") -and -not ($config -eq "Release")){
        Write-Output "Couldn't proceed with -config $config option. Using 'Debug' as default..."
        $config = "Debug"
    }

    # Read and parse the JSON file
    $jsonData = Get-Content -Path $jsonFilePath | ConvertFrom-Json

    # * build.ps1 -config 'Debug'/'Release'
    # Check if new configuration: re-configure all with same generator
    if($config -and -not $generator) {
        if(-not ($config -eq $jsonData.Config)){

            Write-Output "Configuration changed to: $config."

            # Re-config vendor, connector and game; also build vendor and connector
            & "sub_scripts/re_config_all.ps1" -jsonFilePath $jsonFilePath -config $config -generator $jsonData.Generator
            
            # Now we can re-build game itself
            & "sub_scripts/re_build_game.ps1" -config $config

            exit 0
        }
        # ? If configuration didn't changed - treat this command as re-build game:
        else{
            # Now we can re-build game itself
            & "sub_scripts/re_build_game.ps1" -config $jsonData.Config

            exit 0
        }
    }

    # * build.ps1 -generator 'Ninja'/'MinGW'/'VS'
    # Check if another generator: re-configure all for 'Debug'
    if($generator -and -not $config) {
        if(-not ($generator -eq $jsonData.Generator)){

            Write-Output "Generator changed changed to: $generator."

            # Re-config vendor, connector and game; also build vendor and connector
            & "sub_scripts/re_config_all.ps1" -jsonFilePath $jsonFilePath -config "Debug" -generator $generator
            
            # Now we can re-build game itself
            & "sub_scripts/re_build_game.ps1" -config $jsonData.Config

            exit 0
        }
        # ? If generator didn't changed - treat this command as re-build game:
        else{
            # Now we can re-build game itself
            # ! See that we will get same configuration as before from json file
            & "sub_scripts/re_build_game.ps1" -config $jsonData.Config

            exit 0
        }
    }

    # * build.ps1 -generator 'Ninja'/'MinGW'/'VS' -config 'Debug'/'Release'
    # * build.ps1 -config 'Debug'/'Release' -generator 'Ninja'/'MinGW'/'VS'
    if($config -and $generator){
        # Check if the current parameters match the JSON file values
        $isMatch = ($config -eq $jsonData.Config) -and ($generator -eq $jsonData.Generator)

        if (-not $isMatch) {
            # Parameters differ from JSON file values
            Write-Output "Parameters differ. Performing action for differing parameters."
        
            # Re-config vendor, connector and game; also build vendor and connector
            & "sub_scripts/re_config_all.ps1" -jsonFilePath $jsonFilePath -config $config -generator $generator
        
            # Build
            # ? No build step here... just wait for it
        }
        
        # ? If generator and configuration didn't changed - treat this command as re-build game:
        # Now we can re-build game itself
        & "sub_scripts/re_build_game.ps1" -config $jsonData.Config

        exit 0
    }
}
else{
    # ? Some option used for rebuild parameter
    if ($rebuild -and ( -not($rebuild -eq "game") -or -not($rebuild -eq "connector"))) {
        Write-Output "Can't rebuild with option -rebuild $rebuild. No default option will be used..."
        exit 0
    }

    # ? When you need to re-build connector or game with another configuration and/or generator
    # ? you should rebuild all with such options

    # * build.ps1
    # * build.ps1 -rebuild 'game'
    if((-not $rebuild) -or ($rebuild -eq "game")){
        # Simply re-building game
        & "sub_scripts/re_build_game.ps1" -config $jsonData.Config

        exit 0
    }

    # * build.ps1 -rebuild 'connector'
    if($rebuild -eq "connector"){
        # Rebuilding connector first
        & "sub_scripts/re_build_connector.ps1" -config $jsonData.Config -generator $jsonData.Generator
        # Simply re-building game
        & "sub_scripts/re_build_game.ps1" -config $jsonData.Config

        exit 0
    }
}




