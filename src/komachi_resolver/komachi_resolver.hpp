#ifndef KOMACHI_RESOLVER_KOMACHI_RESOLVER_HPP
#define KOMACHI_RESOLVER_KOMACHI_RESOLVER_HPP

#include <iostream>
#include <vector>
#include <optional>
#include <cstddef>
#include "symbol.hpp"
#include "expression_to_revers_polish_notation.hpp"
#include "revers_polish_notation_resolver.hpp"

namespace komachi_resolver{

	template<typename T>
	std::optional<std::vector<Symbol<T>>> komachi_resolver(const std::vector<T>& values, const T& answer){
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
			auto erpn = expression_to_revers_polish_notation( expression );
			auto result = reverse_polish_notation_resolver( erpn );
			if( result && *result == answer ){
				// 合えばvectorを返す
				for(auto&& e : erpn ){
					std::cout << e << " ";
				}
				std::cout << std::endl;
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
}

#endif
