// ---------------------------------------------------------------------------
#ifndef arrayH
#define arrayH

// ---------------------------------------------------------------------------
#include "secref.h"

// ---------------------------------------------------------------------------
/*
 */
// ---------------------------------------------------------------------------
namespace dynlib {
	namespace secref {

		template<class Object>
		class Array {
		private:
			int arr_size;
			Reference<Object> *arr;

		public:
			Array() {
				arr_size = 0;
				arr = 0;
			}

			Array(int size) {
				if (arr_size > 0) {
					arr_size = size;
					arr = new Reference<Object>[arr_size];
				}
				else {
					arr_size = 0;
					arr = 0;
				}
			}

			~Array() {
				if (arr_size > 0) {
					delete[]arr;
					arr_size = 0;
					arr = 0;
				}
			}

			Reference<Object>&operator[](int index)const {
				return arr[index];
			}

			int size()const {
				return arr_size;
			}

			bool isNull()const {
				return arr == 0;
			}
		};
	}
}
// ---------------------------------------------------------------------------
#endif
