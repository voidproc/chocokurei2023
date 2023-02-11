if ((-not $Env:ASEPRITE_EXE_PATH) -or (-not (Test-Path $Env:ASEPRITE_EXE_PATH))) {
  Write-Host "ŠÂ‹«•Ï”'ASEPRITE_EXE_PATH'‚ÉAseprite.exe‚ÌƒpƒX‚ğİ’è‚µ‚Ä‚­‚¾‚³‚¢."
  exit
}

ls *.aseprite | %{ start $Env:ASEPRITE_EXE_PATH -Arg "-b",([string]$_),"--sheet","$($_.BaseName).png" }
