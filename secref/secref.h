// ---------------------------------------------------------------------------
#ifndef secrefH
#define secrefH

// ---------------------------------------------------------------------------
/*
----class Reference<Object>{
----public:
--------Reference();
--------Reference(Object *data);
--------Reference(Reference &ref);
--------~Reference();
--------Object& get()const;
--------Object* operator->()const;
--------Reference& operator = (Reference & ref);
--------Reference& operator = (Object * data);
--------bool operator == (Reference & ref)const;
--------bool operator != (Reference & ref)const;
--------bool isNull()const;
--------bool hasCopy()const;
--------int getCountOfCopy()const;
--------void remove();
};
 */
// ---------------------------------------------------------------------------
namespace dynlib {
	namespace secref {

		template<class Object>
		class Reference {
		private:
			Object *data;
			Reference *prev;
			Reference *next;

			void copy(const Reference &ref) {
				Reference *r = const_cast<Reference*>(&ref);
				if (r->data) {
					data = r->data;
					next = r->next;
					prev = r;
					if (r->next) {
						r->next->prev = this;
					}
					r->next = this;
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

			Reference(const Object *data) {
				empty();
				this->data = const_cast<Object*>(data);
			}

			Reference(const Reference &ref) {
				copy(ref);
			}

			~Reference() {
				remove();
			}

			Object& get()const {
				return *data;
			}

			template<class ExtendObject>
			ExtendObject& get()const {
				return *((ExtendObject*)(const_cast<Object*>(data)));
			}

			Object* operator->()const {
				return data;
			}

			Reference& operator = (const Reference & ref) {
				if (this != &ref) {
					remove();
					copy(ref);
				}
				return *this;
			}

			Reference& operator = (const Object * data) {
				if (this->data != data) {
					remove();
					this->data = const_cast<Object*>(data);
				}
				return *this;
			}

			bool operator == (const Reference & ref)const {
				return data == ref.data;
			}

			bool operator != (const Reference & ref)const {
				return data != ref.data;
			}

			bool isNull()const {
				return data == 0;
			}

			bool hasCopy()const {
				return prev || next;
			}

			int getReferencesCount()const {
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

			void removeAll() {
				while (prev) {
					prev->remove();
				}
				while (next) {
					next->remove();
				}
				remove();
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
