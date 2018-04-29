TR2Input='tr2.inHT6M'

./Clean.sh

Time1=$(date)

# EXECUTING COMMANDS
../tr2 < $TR2Input
gstoeps -ar tr2-HT6M.gs

Time2=$(date)

echo " "
echo "============================================" 
echo "Input file    : $TR2Input"
echo "Starting time : $Time1"
echo "Finishing time: $Time2"
echo "============================================" 
echo " "
