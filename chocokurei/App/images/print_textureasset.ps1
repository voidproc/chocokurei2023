ls *.png | %{ "TextureAsset::Register(U`"$($_.BaseName)`", Resource(U`"images/$($_.Name)`"));" }
