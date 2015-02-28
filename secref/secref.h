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

			Object* operator->()const {
				return data;
			}

			Reference& operator = (Reference & ref) {
				if (this != &ref) {
					remove();
					copy(ref);
				}
				return *this;
			}

			Reference& operator = (Object * data) {
				if (this->data != data) {
					remove();
					this->data = data;
				}
				return *this;
			}

			bool operator == (Reference & ref)const {
				return data == ref.data;
			}

			bool operator != (Reference & ref)const {
				return data != ref.data;
			}

			bool isNull()const {
				return data == 0;
			}

			bool hasCopy()const {
				return prev || next;
			}

			int getCountOfCopy()const {
				int count = 0;
				if (!isNull()) {
					count = 1;
					Reference *ref = prev;
					while (ref) {
						++count;
						ref = ref->prev;
					}
					ref = next;
					while (ref) {
						++count;
						ref = ref->next;
					}
				}
				return count;
			}

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
				empty();
			}

		};
	}
}
// ---------------------------------------------------------------------------
#endif
