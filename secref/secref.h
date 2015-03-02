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

		class IReference {
		private:
			friend void __setNext(IReference &iref, void *next);
			friend void __setPrev(IReference &iref, void *prev);
			friend void __setData(IReference &iref, void *data);

			friend void* __getNext(const IReference &iref);
			friend void* __getPrev(const IReference &iref);
			friend void* __getData(const IReference &iref);

			void *data;
			void *prev;
			void *next;
		};

		void __setNext(IReference &iref, void *next) {
			iref.next = next;
		}

		void __setPrev(IReference &iref, void *prev) {
			iref.prev = prev;
		}

		void __setData(IReference &iref, void *data) {
			iref.data = data;
		}

		void* __getNext(const IReference &iref) {
			return iref.next;
		}

		void* __getPrev(const IReference &iref) {
			return iref.prev;
		}

		void* __getData(const IReference &iref) {
			return iref.data;
		}
	}
}

// ---------------------------------------------------------------------------
namespace dynlib {
	namespace secref {

		template<class Object>
		class Reference : public IReference {
		private:

			void copy(const Reference &ref) {
				Reference *r = const_cast<Reference*>(&ref);
				if (__getData(*r)) {
					__setData(*this, __getData(*r));
					__setNext(*this, __getNext(*r));
					__setPrev(*this, __getPrev(*r));
					if (__getNext(*r)) {
						__setPrev(*((Reference*)__getNext(*(Reference*)r)),
							(void*)this);
					}
					__setNext(*r, (void*)this);
				}
				else {
					empty();
				}
			}

			void empty() {
				__setData(*this, (void*)0);
				__setPrev(*this, (void*)0);
				__setNext(*this, (void*)0);
			}

		public:
			Reference() {
				empty();
			}

			Reference(const Object * data) {
				empty();
				__setData(*this, const_cast<Object*>(data));
			}

			Reference(const Reference & ref) {
				copy(ref);
			}

			~Reference() {
				remove();
			}

			Object& get()const {
				return *((Object*)(__getData(*this)));
			}

			template<class ExtendObject>
			ExtendObject& get()const {
				return *
					((ExtendObject*)
					(const_cast<Object*>((Object*)__getData(*this))));
			}

			Object* operator->()const {
				return(Object*)__getData(*this);
			}

			Reference& operator = (const Reference & ref) {
				if (this != &ref) {
					remove();
					copy(ref);
				}
				return *this;
			}

			Reference& operator = (const Object * data) {
				if (__getData(*this) != data) {
					remove();
					__setData(*this, const_cast<Object*>(data));
				}
				return *this;
			}

			bool operator == (const Reference & ref)const {
				return __getData(*this) == __getData(ref);
			}

			bool operator != (const Reference & ref)const {
				return data != ref.data;
			}

			bool isNull()const {
				return __getData(*this) == 0;
			}

			bool hasCopy()const {
				return prev || next;
			}

			int getReferencesCount()const {
				int count = 0;
				if (!isNull()) {
					count = 1;
					IReference *ref = (IReference*)__getPrev(*this);
					while (ref) {
						++count;
						ref = (IReference*)__getPrev(*ref);
					}
					ref = (IReference*)__getNext(*this);
					while (ref) {
						++count;
						ref = (IReference*)__getNext(*ref);
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
				if (__getPrev(*this)) {
					if (__getNext(*this)) {
						__setNext(*((IReference*)__getPrev(*this)),
							__getNext(*this));
						__setPrev(*((IReference*)__getNext(*this)),
							__getPrev(*this));
					}
					else {
						__setNext(*((IReference*)__getPrev(*this)), (void*)0);
					}
				}
				else {
					if (__getNext(*this)) {
						__setPrev(*((IReference*)__getNext(*this)), (void*)0);
					}
					else {
						if (__getData(*this)) {
							delete(Object*)__getData(*this);
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
