ls *.png | ?{ -not ($_.BaseName -match "^_") } | %{ "TextureAsset::Register(U`"$($_.BaseName)`", Resource(U`"images/$($_.Name)`"));" }
