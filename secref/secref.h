// ---------------------------------------------------------------------------
#ifndef secrefH
#define secrefH

// ---------------------------------------------------------------------------
namespace dynlib {
	namespace secref {

		template<class Object>
		class Reference {
		private:
			Object *data;
			Reference *prev;
			Reference *next;

			void remove() {
				if (prev) {
					if (next) {
						prev->next = next;
						next->prev = prev;
					}
					else {
						prev->next = 0;
					}
				}
				else {
					if (next) {
						next->prev = 0;
					}
					else {
						if (data) {
							delete data;
						}
					}
				}
			}

			void copy(Reference &ref) {
				if (ref.data) {
					data = ref.data;
					next = ref.next;
					prev = &ref;
					if (ref.next) {
						ref.next->prev = this;
					}
					ref.next = this;
				}
				else {
					empty();
				}
			}

			void empty() {
				data = 0;
				prev = 0;
				next = 0;
			}

		public:
			Reference() {
				empty();
			}

			Reference(Object *data) {
				empty();
				this->data = data;
			}

			Reference(Reference &ref) {
				copy(ref);
			}

			~Reference() {
				remove();
			}

			Object& get()const {
				return *data;
			}

			Reference& operator = (Reference & ref) {
				if (this != &ref) {
					remove();
					copy(ref);
				}
				return *this;
			}

			bool isEmpty()const {
				return data == 0;
			}

		};
	}
}
// ---------------------------------------------------------------------------
#endif
