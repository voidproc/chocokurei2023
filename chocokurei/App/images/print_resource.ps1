ls *.png | %{ "Resource(images/$($_.Name))" }
