#include <iostream>
#include <stack>
#include <vector>
#include <optional>
#include <string>
#include <cstddef>
#include "symbol.hpp"

template<typename T>
T stack_result(const std::vector<momoka::Symbol<T>>& expression){
	// 基本的な逆ポーランド記法の演算の実装
	// wikiマン
	using namespace momoka;
	std::stack<T> stack;
	T accumulator = 0;

	for(auto&& e : expression){
		if( e.has_data() ){
			stack.push(e.get_data());
		}else{
			accumulator = stack.top();
			stack.pop();
			switch(e.get_operator_symbol()){
				case operator_symbol::plus :
					accumulator += stack.top();
					break;
				case operator_symbol::minus :
					accumulator = stack.top() - accumulator;
					break;
				case operator_symbol::times :
					accumulator *= stack.top();
					break;
				case operator_symbol::divide : 
					accumulator = stack.top() / accumulator;
					break;
			}
			stack.pop();
			stack.push(accumulator);
		}
	}

	return stack.top();
}

template<typename T>
std::vector<momoka::Symbol<T>> expression_to_revers_polish_notation(const std::vector<momoka::Symbol<T>>& expression){
	// 通常の式を似非逆ポーランド記法にする
	using namespace momoka;
	std::vector<bool> use_flag( expression.size(), false );
	std::vector<Symbol<T>> rpn;

	// 優先順位の高い式を格納する
	for(std::size_t i = 0u; i < expression.size(); i++){
		// *か/なら左辺を右辺を取ってくる
		if( expression[i].has_operator_symbol() && !use_flag[i] && 
				(expression[i].get_operator_symbol() == operator_symbol::times ||
				 expression[i].get_operator_symbol() == operator_symbol::divide))
		{
			// 左辺、右辺、オペレータをそれぞれ格納してしまう
			rpn.emplace_back( expression[i - 1].get_data() );
			rpn.emplace_back( expression[i + 1].get_data() );
			rpn.emplace_back( expression[i].get_operator_symbol() );

			use_flag[i - 1] = true;
			use_flag[i] = true;
			use_flag[i + 1] = true;
		}
	}

	// 順々に式を格納していく
	for(std::size_t i = 0u; i < expression.size(); i++){
		if( !use_flag[i] ){
			// 値の場合
			if( expression[i].has_data() ){
				// そのまま格納
				rpn.emplace_back( expression[i].get_data() );
			}else{
				// 式の場合
				// 次の値が未使用であれば先にそれを格納
				if( (i + 1) != expression.size() && !use_flag[i + 1]){
					rpn.emplace_back( expression[i + 1].get_data() );
					use_flag[i + 1] = true;
				}
				// 式を格納
				rpn.emplace_back( expression[i].get_operator_symbol() );
			}
			use_flag[i] = true;
		}
	}

	return rpn;
}

template<typename T>
std::optional<std::vector<momoka::Symbol<T>>> resulver(const std::vector<T>& values, const T& answer){
	using namespace momoka;
	std::vector<operator_symbol> operators{
		operator_symbol::plus,
		operator_symbol::minus,
		operator_symbol::times,
		operator_symbol::divide
	};

	// 与えられた数-1分の演算子を用意
	std::vector<std::size_t> operator_index( values.size() - 1, 0 );

	while(true){
		// 連結
		std::vector<Symbol<T>> expression;
		for(std::size_t i = 0u; i < values.size(); i++){
			expression.emplace_back( values[i] );
			// 最後じゃなければ式を格納
			if( i != values.size() - 1 ){
				expression.emplace_back( operators[ operator_index[i] ] );
			}
		}

		// 答えがあうかね？
		if( stack_result( expression_to_revers_polish_notation( expression )) == answer ){
			// 合えばvectorを返す
			return expression;
		}

		// あってない場合は1ずつ繰り上げ
		bool is_break = false;
		for(std::size_t i = 0; i < operator_index.size(); i++){
			operator_index[i] += 1;
			if( operator_index[i] >= operators.size() ){
				// 最後のタイミングならやめ
				if( i == operator_index.size() - 1 ){
					is_break = true;
					break;
				}
				operator_index[i] = 0;
			}else{
				break;
			}
		}

		if(is_break){
			break;
		}
	}

	return std::nullopt;
}

int main(int argc, char* argv[]){
	using namespace momoka;
	std::vector<int> values;
	for(int i = 1; i < argc - 1; i++ ){
		values.emplace_back( std::stoi(std::string(argv[i])) );
	}
	int answer = std::stoi( argv[argc - 1] );
	
	auto result = resulver(values, answer);
	if(result){
		for(auto&& e : *result){
			if( e.has_data() ){
				std::cout << e.get_data() << " ";		
			}else{
				std::cout << e.get_operator_symbol()  << " ";
			}
		}

		std::cout << "=" << answer  << std::endl;	
	}else{
		std::cout << "no match" << std::endl;
	}

	return 0;
}
