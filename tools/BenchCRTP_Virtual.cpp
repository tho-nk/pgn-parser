
#include <iostream>
#include <vector>
#include <memory>
#include <variant>
#include <chrono>

class BasePtr {
public:
	BasePtr(int nb = 1) : nb_(nb) {};
  	virtual int Process() const = 0;
  	~BasePtr() = default;
protected:
	int nb_;
};

class Algo_1_Ptr : public BasePtr {
public:
	Algo_1_Ptr(int nb) : BasePtr(nb) {}
  	int Process() const override {
    	int sum = 0;
    	for (int i = 0; i < nb_; ++i) {	sum += i; }
		return sum;
  	}
};

class Algo_2_Ptr : public BasePtr {
public:
	Algo_2_Ptr(int nb) : BasePtr(nb) {}
  	int Process() const override {
    	int sum = 0;
    	for (int i = 0; i < nb_; ++i) { sum += i*2; }
		return sum/2;
	}
};

class Algo_3_Ptr : public BasePtr {
public:
	Algo_3_Ptr(int nb) : BasePtr(nb) {}
  	int Process() const override {
    	int sum = 0;
    	for (int i = 0; i < nb_; ++i) { sum += i*3; }
		return sum/3;
	}
};

int ProcessPtr(const BasePtr* ptr) {
  	return ptr->Process();
}

template <typename Concrete> class BaseCRTP {
public:
	int Process() const { return self().Process_(); }
protected:
	friend Concrete;
	BaseCRTP(int nb) : nb_(nb) {};
	Concrete &self() { return *static_cast<Concrete *>(this); }
	Concrete const &self() const { return *static_cast<Concrete const *>(this); }
	int nb_;
};

class Algo_1_CRTP : public BaseCRTP<Algo_1_CRTP> {
public:
  	Algo_1_CRTP(int nb) : BaseCRTP(nb) {}
private:
	friend class BaseCRTP<Algo_1_CRTP>;
	int Process_() const {
		int sum = 0;
		for (int i = 0; i < nb_; ++i) {	sum += i; }
		return sum;
	}
};

class Algo_2_CRTP : public BaseCRTP<Algo_2_CRTP> {
public:
  	Algo_2_CRTP(int dem) : BaseCRTP(dem) {}
private:
  	friend class BaseCRTP<Algo_2_CRTP>;
  	int Process_() const {
		int sum = 0;
		for (int i = 0; i < nb_; ++i) { sum += i*2; }
		return sum/2;
	}
};

class Algo_3_CRTP : public BaseCRTP<Algo_3_CRTP> {
public:
  	Algo_3_CRTP(int dem) : BaseCRTP(dem) {}
private:
  	friend class BaseCRTP<Algo_3_CRTP>;
  	int Process_() const {
		int sum = 0;
		for (int i = 0; i < nb_; ++i) {	sum += i*3; }
		return sum/3;
	}
};

template <typename T>
int ProcessTemplate(const T& piece) {
  	return piece.Process();
}

int main() {
	int nb = 1000000;
  	std::vector<BasePtr*> v_pieces;
	v_pieces.push_back(new Algo_1_Ptr(1000));
	v_pieces.push_back(new Algo_2_Ptr(2000));
	v_pieces.push_back(new Algo_3_Ptr(3000));

	v_pieces.push_back(new Algo_1_Ptr(100));
	v_pieces.push_back(new Algo_2_Ptr(200));
	v_pieces.push_back(new Algo_3_Ptr(300));

	v_pieces.push_back(new Algo_1_Ptr(10));
	v_pieces.push_back(new Algo_2_Ptr(20));
	v_pieces.push_back(new Algo_3_Ptr(30));

	v_pieces.push_back(new Algo_1_Ptr(1000));
	v_pieces.push_back(new Algo_2_Ptr(2000));
	v_pieces.push_back(new Algo_3_Ptr(3000));

	v_pieces.push_back(new Algo_1_Ptr(100));
	v_pieces.push_back(new Algo_2_Ptr(200));
	v_pieces.push_back(new Algo_3_Ptr(300));

	v_pieces.push_back(new Algo_1_Ptr(10));
	v_pieces.push_back(new Algo_2_Ptr(20));
	v_pieces.push_back(new Algo_3_Ptr(30));
	

	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < nb; ++i) {
		for (auto piece : v_pieces) {
			//auto result = ProcessPtr(piece);
			volatile auto result = piece->Process();
		}
	}
	for (auto piece : v_pieces) {
		delete piece;
	}
	auto end = std::chrono::high_resolution_clock::now();
	double time_v = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

	using Piece = std::variant<Algo_1_CRTP, Algo_2_CRTP, Algo_3_CRTP>;
	std::vector<Piece> c_pieces;
	c_pieces.emplace_back(Algo_1_CRTP{1000});
	c_pieces.emplace_back(Algo_2_CRTP{2000});
	c_pieces.emplace_back(Algo_3_CRTP{3000});

	c_pieces.emplace_back(Algo_1_CRTP{100});
	c_pieces.emplace_back(Algo_2_CRTP{200});
	c_pieces.emplace_back(Algo_3_CRTP{300});

	c_pieces.emplace_back(Algo_1_CRTP{10});
	c_pieces.emplace_back(Algo_2_CRTP{20});
	c_pieces.emplace_back(Algo_3_CRTP{30});

	c_pieces.emplace_back(Algo_1_CRTP{1000});
	c_pieces.emplace_back(Algo_2_CRTP{2000});
	c_pieces.emplace_back(Algo_3_CRTP{3000});

	c_pieces.emplace_back(Algo_1_CRTP{100});
	c_pieces.emplace_back(Algo_2_CRTP{200});
	c_pieces.emplace_back(Algo_3_CRTP{300});

	c_pieces.emplace_back(Algo_1_CRTP{10});
	c_pieces.emplace_back(Algo_2_CRTP{20});
	c_pieces.emplace_back(Algo_3_CRTP{30});

	auto start2 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < nb; ++i) {
		for (auto piece : c_pieces) {
			std::visit([](auto &p) {
				//auto result = ProcessTemplate(p);
				volatile auto result = p.Process();
			}, piece);
		}
  	}
	auto end2 = std::chrono::high_resolution_clock::now();
	double time_c = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2).count();
	std::cout << "Time for virtual: " << time_v << " ns" << std::endl;
	std::cout << "Time for CRTP: " << time_c << " ns" << std::endl;

	return 0;
}