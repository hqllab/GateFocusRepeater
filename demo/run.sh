#!/run/current-system/sw/bin/bash

#SBATCH -o %J.out
#SBATCH -e %J.err
#SBATCH -x luna
#SBATCH --mem=20G

echo 'Run on:' `hostname`
echo 'Start at: ' `date`
echo 'Start MC Simulation at: ' `date`
nix run "git+ssh://git@github.com/WonderLand-HqDev/tech-nix.git?ref=main"#gate.gate9_4Modify -- main.mac
echo 'Finish MC Simulation at: ' `date`
echo 'Finish at: ' `date`
