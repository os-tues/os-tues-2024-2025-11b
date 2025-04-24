args="GO SH O"

echo "Hello, $USER!"
echo "Args are $@"
echo $1 + $2

for command in "touch example.txt" "stat example.txt" "ls"; do
  # for command in $args; do
  echo "command: $command"
  $command
done

echo -------------------

ls_output=$("ls")
echo $ls_output

# ls .. | grep "multi"
