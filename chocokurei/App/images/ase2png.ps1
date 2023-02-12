if ((-not $Env:ASEPRITE_EXE_PATH) -or (-not (Test-Path $Env:ASEPRITE_EXE_PATH))) {
  Write-Host "環境変数'ASEPRITE_EXE_PATH'にAseprite.exeのパスを設定してください."
  exit
}

ls *.aseprite | %{ start $Env:ASEPRITE_EXE_PATH -Arg "-b",([string]$_),"--sheet","$($_.BaseName).png" }
