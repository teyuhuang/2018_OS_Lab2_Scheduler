MAIN = sched
Include = utility.cpp scheduler.cpp process.cpp DES.cpp

INPUT_BASE = ./lab2_assign/
exec: $(MAIN)
	@./$^ -sF $(INPUT_BASE)input0 $(INPUT_BASE)rfile
	@ rm $^
$(MAIN): $(MAIN).cpp $(Include)
	@g++ -std=c++0x $^ -o $(MAIN)
cm: $(MAIN).cpp $(Include)
	@echo "=======IMPORTANT======"
	@echo "Pleas use gcc 6.2.0 or above"
	@echo "module load gcc-6.2.0"
	@echo "executible will be in ./src/ as sched"
	@echo "======================"
	@mkdir src
	@g++ -std=c++0x $^ -o ./src/$(MAIN)
test: test.cpp
	@g++ -std=c++0x $^ -o test
	@./test
	@rm test
clear: $(MAIN)
	rm $^
	rm -rf src
loadModule:
	module load gcc-6.2.0
unloadModule:
	module unload gcc-6.2.0
