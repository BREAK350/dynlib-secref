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
						delete data;
					}
				}
			}

			void copy(Reference &ref) {
				data = ref.data;
				next = ref.next;
				prev = &ref;
				if (ref.next) {
					ref.next->prev = this;
				}
				ref.next = this;
			}

		public:
			Reference() {
				data = new Object();
				prev = 0;
				next = 0;
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
				remove();
				copy(ref);
				return *this;
			}

		};
	}
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
#endif
