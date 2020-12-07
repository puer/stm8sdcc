https://yurovsky.github.io/2017/04/18/stm8-boot-code-sdcc.html


# read option bytes

stm8flash -c stlink -p stm8s003?3 -s opt -r opt.bin

# write option bytes

after read and edit (xxd) the option binary file, write them back

stm8flash -c stlink -p stm8s003?3 -s opt -w opt.bin


here we set AFR0 to use PC6 as Timer1 Ch1 
