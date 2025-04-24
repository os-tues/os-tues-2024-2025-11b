echo $@

while [[ 1 ]]; do
  clear
  # exec $@
  $@
  sleep 2
done
