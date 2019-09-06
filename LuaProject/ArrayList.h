#ifndef ARRAYLIST_H
#define ARRAYLIST_H

//Bättre resize
//DeleteAll

template <class T>
class ArrayList
{
private:
	T* *elements;
	int nrOfElements;
	int cap;
	int expandInterval;

	void initialize(int start = 0);
	void freeMemory();
	void expand();
	void copy(const ArrayList<T>& list);
public:
	ArrayList(int startcap = 10, int expandInterval = 10);
	ArrayList(const ArrayList<T>& list);
	~ArrayList();
	ArrayList<T>& operator=(const ArrayList<T>& list);
	T& operator[](int index) const;

	void resize(int size);
	void insertAt(int pos, const T& element);
	T elementAt(int pos) const;
	T removeAt(int pos);
	int size() const;

};

template <class T>
void ArrayList<T>::initialize(int start)
{
	for (int i = start; i < this->cap; i++)
	{
		this->elements[i] = nullptr;
	}
}

template <class T>
void ArrayList<T>::freeMemory()
{
	for (int i = 0; i < this->nrOfElements; i++)
	{
		delete this->elements[i];
	}

	delete[] this->elements;
}

template <class T>
void ArrayList<T>::expand()
{
	this->cap += this->expandInterval;
	T* *temp = new T*[this->cap];

	for (int i = 0; i < this->nrOfElements; i++)
	{
		temp[i] = this->elements[i];
	}

	delete[] this->elements;
	this->elements = temp;
}

template <class T>
void ArrayList<T>::copy(const ArrayList<T>& list)
{
	this->nrOfElements = list.nrOfElements;
	this->cap = list.cap;
	this->expandInterval = list.expandInterval;

	this->elements = new T*[list.cap];
	for (int i = 0; i < list.nrOfElements; i++)
	{
		this->elements[i] = new T(*list.elements[i]);
	}
	this->initialize(list.nrOfElements);
}

template <class T>
ArrayList<T>::ArrayList(int startcap, int expandInterval)
{
	this->cap = startcap;
	this->expandInterval = expandInterval;
	this->nrOfElements = 0;
	this->elements = new T*[startcap];
	initialize();
}

template <class T>
ArrayList<T>::ArrayList(const ArrayList<T>& list)
{
	this->copy(list);
}

template <class T>
ArrayList<T>::~ArrayList()
{
	this->freeMemory();
}

template <class T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& list)
{
	if (this != &list)
	{
		this->freeMemory();
		this->copy(list);
	}

	return *this;
}

template<class T>
T& ArrayList<T>::operator[](int index) const
{
	return *this->elements[index];
}

template<class T>
void ArrayList<T>::resize(int size)
{
	if (size < cap)
		throw "Can't resize to lesser";


	this->cap = size;
	T* *temp = new T*[this->cap];

	for (int i = 0; i < this->nrOfElements; i++)
	{
		temp[i] = this->elements[i];
	}

	delete[] this->elements;
	this->elements = temp;
}

template <class T>
void ArrayList<T>::insertAt(int pos, const T& element)
{
	if (cap < 0 || pos > this->nrOfElements)
		throw "Out-of-bounds";

	if (this->nrOfElements >= this->cap)
		this->expand();

	if (this->nrOfElements == pos)
		this->elements[pos] = new T(element);

	else
	{
		for (int i = this->nrOfElements; i >= pos; i--)
		{
			this->elements[i + 1] = this->elements[i];
		}

		this->elements[pos] = new T(element);
	}

	this->nrOfElements++;

}

template<class T>
T ArrayList<T>::elementAt(int pos) const
{
	if (this->nrOfElements == 0)
		throw "List is empty";

	if (cap < 0 || pos >= this->nrOfElements)
		throw "Out-of-bounds";

	return *this->elements[pos];
}

template<class T>
T ArrayList<T>::removeAt(int pos)
{
	if (this->nrOfElements == 0)
		throw "List is empty";

	if (cap < 0 || pos >= this->nrOfElements)
		throw "Out-of-bounds";

	T item = *this->elements[pos];
	delete this->elements[pos];

	for (int i = pos; i < this->nrOfElements; i++)
	{
		this->elements[i] = this->elements[i + 1];
	}

	this->nrOfElements--;

	return item;
}

template<class T>
int ArrayList<T>::size() const
{
	return this->nrOfElements;
}

#endif