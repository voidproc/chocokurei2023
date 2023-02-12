ls *.png | ?{ -not ($_.BaseName -match "^_") } | %{ "Resource(images/$($_.Name))" }
