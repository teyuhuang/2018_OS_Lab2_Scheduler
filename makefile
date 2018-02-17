MAIN = sched
Include = utility.cpp scheduler.cpp process.cpp DES.cpp

INPUT_BASE = ./lab2_assign/
exec: $(MAIN)
	@./$^ -v -sS $(INPUT_BASE)input0 $(INPUT_BASE)rfile
	@ #./$^ -sF $(INPUT_BASE)input0 $(INPUT_BASE)rfile
	@ #./$^ -v -sF $(INPUT_BASE)input0 $(INPUT_BASE)rfile
	@rm $^
$(MAIN): $(MAIN).cpp $(Include)
	@g++ -std=c++0x $^ -o $(MAIN)
clear: $(MAIN)
	rm $^
loadModule:
	module load gcc-6.2.0
unloadModule:
	module unload gcc-6.2.0
# @echo "=======IMPORTANT======"
# @echo "Pleas use gcc 6.2.0 or above"
# @echo "module load gcc-6.2.0"
# @echo "======================"