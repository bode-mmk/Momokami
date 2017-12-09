#include <string>
#include <iostream>
#include <optional>

namespace momoka{
	enum struct operator_symbol{
		plus,
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
	private:
		std::optional<T> data_;
		operator_symbol operator_;	
	};	
}
