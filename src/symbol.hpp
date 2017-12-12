#include <string>
#include <iostream>
#include <optional>

namespace momoka{

	// 内部のintは優先順位として扱う
	enum struct operator_symbol : int{
		plus = 0,
		minus,
		times,
		divide
	};

	inline std::ostream& operator<<(std::ostream& os, operator_symbol symbol){
		switch(symbol){
			case operator_symbol::plus:
				os << "+";
				break;
			case operator_symbol::minus:
				os << "-";
				break;
			case operator_symbol::times:
				os << "*";
				break;
			case operator_symbol::divide:
				os << "/";
				break;
		}
		return os;
	}
	
	// 演算子優先順位の比較
	inline bool operator<(const operator_symbol left, const operator_symbol right){
		// 両方+か-なら優先順位は同等なのでfalse
		return
			( static_cast<int>(left) < 2 && static_cast<int>(right) < 2) ? 
			false : 
			( static_cast<int>(left) < static_cast<int>(right) );
	}
	inline bool operator>(const operator_symbol left, const operator_symbol right){
		return right < left;
	}
	inline bool operator<=(const operator_symbol left,const operator_symbol right){
		return !(left > right);
	}
	inline bool operator>=(const operator_symbol left,const operator_symbol right){
		return !(left < right);
	}

	template<typename T>
	class Symbol{
	public:
		Symbol() = default;
		virtual ~Symbol() = default;

		Symbol(const T& data) :
			data_( data ){}

		Symbol(const operator_symbol& op) :
			operator_( op ){}
		
		bool has_data() const {
			return (data_.has_value());	
		}
		bool has_operator_symbol() const {
			return !(data_.has_value());
		}
		auto get_data() const {
			return *data_;
		}
		auto get_operator_symbol() const {
			return operator_;
		}
		void set_data(const T& data){
			data_ = data;
		}
		void set_operator(const operator_symbol& os){
			operator_ = os;
			data_ = std::nullopt;
		}		
		
		template<typename E>
		friend std::ostream& operator<<(std::ostream& os,const Symbol<E>& symbol);
	private:
		std::optional<T> data_;
		operator_symbol operator_;	
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& os,const  Symbol<T>& symbol){
		if( symbol.has_data() ){
			os << symbol.get_data();
		}else{
			os << symbol.get_operator_symbol();
		}
		return os;
	}
}
