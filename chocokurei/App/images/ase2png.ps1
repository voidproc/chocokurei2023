if ((-not $Env:ASEPRITE_EXE_PATH) -or (-not (Test-Path $Env:ASEPRITE_EXE_PATH))) {
  Write-Host "���ϐ�'ASEPRITE_EXE_PATH'��Aseprite.exe�̃p�X��ݒ肵�Ă�������."
  exit
}

ls *.aseprite | %{ start $Env:ASEPRITE_EXE_PATH -Arg "-b",([string]$_),"--sheet","$($_.BaseName).png" }
