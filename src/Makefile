FLAGS = g++ -g -std=c++17 -Wall -Werror -Wextra
LIBS=-lgtest -lgmock -pthread


all: tests demo

self_balancing_binary_search_tree.a: clean
	$(FLAGS) -c model/structures/*.cc model/helpers/*.cc model/self_balancing_tree/*.cc
	ar rcs self_balancing_binary_search_tree.a *.o
	ranlib self_balancing_binary_search_tree.a

hash_table.a: clean
	$(FLAGS) -c model/structures/*.cc model/helpers/*.cc model/hash_table/*.cc
	ar rcs hash_table.a *.o
	ranlib hash_table.a

b_plus_tree.a: clean
	$(FLAGS) -c model/structures/*.cc model/helpers/*.cc model/b_plus_tree/*.cc model/b_plus_tree/nodes/*.cc
	ar rcs b_plus_tree.a *.o
	ranlib b_plus_tree.a

tests: self_balancing_binary_search_tree.a hash_table.a b_plus_tree.a
	$(FLAGS) $(LIBS) self_balancing_binary_search_tree.a hash_table.a b_plus_tree.a test/test.cc
	CK_FORKS=no leaks -atExit -- ./a.out

style:
	cp ../materials/linters/.clang-format ./
	clang-format -n ./*.cc ./*/*.cc ./*/*.h ./*/*/*.cc ./*/*/*.h ./*/*/*/*.cc ./*/*/*/*.h
	@rm -rf .clang-format

clean:
	@rm -rf *.o *.a a.out* .clang-format
	@rm -rf test/test.h.gch model/*.h.gch model/*/*.h.gch

demo: self_balancing_binary_search_tree.a hash_table.a b_plus_tree.a
	g++ $(WWW) main.cc view/view.cc controller/controller.cc model/model/model.cc model/runtime_testing/runtime_testing.cc \
	self_balancing_binary_search_tree.a hash_table.a b_plus_tree.a
	./a.out
