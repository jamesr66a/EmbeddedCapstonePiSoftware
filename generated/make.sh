for file in `ls | grep .pbo$`; do
  ~/embedded2016/protobono/proboc < $file  
done
