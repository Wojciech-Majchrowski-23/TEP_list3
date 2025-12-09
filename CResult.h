#pragma once
#include <vector>

class CTree;
class CError;

template <typename T, typename E>
class CResult
{
private:
	T* pc_value;
	std::vector<E*> v_errors;

public:
	CResult(const T& cValue)
	{
		pc_value = new T(cValue);
	}

	CResult(E* pcError)
	{
		pc_value = NULL;
		if (pcError != NULL)
		{
			v_errors.push_back(pcError);
		}
	}

	CResult(std::vector<E*>& vErrors)
	{
		pc_value = NULL;
		v_errors = vErrors;
		vErrors.clear(); // ten wektor juz nie ma dostepu do errorow (zostaly na stosie, ale strzalki zostaly odciete)
	}

	CResult(const CResult<T, E>& cOther)
	{
		if (cOther.pc_value != NULL)
		{
			pc_value = new T(*cOther.pc_value);
		}
		else { pc_value = NULL; }
		for (int i = 0; i < cOther.v_errors.size(); i++)
		{
			if (cOther.v_errors[i] != NULL)
			{
				v_errors.push_back(new E(*cOther.v_errors[i]));
			}
		}
	}

	~CResult()
	{
		if (pc_value != NULL)
		{
			delete pc_value;
		}
		for (int i = 0; i < v_errors.size(); i++)
		{
			if (v_errors[i] != NULL)
			{
				delete v_errors[i];
			}
		}
		v_errors.clear(); // to trzeba jeszcze zrobic, bo sa w nim teraz smieci
	}

	static CResult<T, E> cOk(const T& cValue)
	{
		return CResult<T, E>(cValue);
	}
	static CResult<T, E> cFail(E* pcError)
	{
		return CResult<T, E>(pcError);
	}
	static CResult<T, E> cFail(std::vector<E*>& vErrors)
	{
		return CResult<T, E>(vErrors);
	}
	static CResult<CTree, CError>cFail(const CTree& cTree)
	{
		return CResult::cOk(cTree);
	}

	CResult<T, E>& operator=(const CResult<T, E>& cOther)
	{
		if (this == &cOther)
		{
			return *this;
		}

		if (pc_value != NULL) delete pc_value;
		for (size_t i = 0; i < v_errors.size(); i++) delete v_errors[i];
		v_errors.clear();

		if (cOther.pc_value != NULL)
		{
			pc_value = new T(*cOther.pc_value);
		}
		else
		{
			pc_value = NULL;
		}

		for (size_t i = 0; i < cOther.v_errors.size(); i++)
		{
			if (cOther.v_errors[i] != NULL)
			{
				v_errors.push_back(new E(*cOther.v_errors[i]));
			}
		}

		return *this;
	}

	bool bIsSuccess() const
	{
		return (v_errors.empty() && pc_value != NULL);
	}

	T cGetValue() const
	{
		if (pc_value != NULL) return *pc_value;
		return T();
	}

	const std::vector<E*>& vGetErrors() const
	{
		return v_errors;
	}
};

template <typename E>
class CResult<void, E>
{
private:
	std::vector<E*> v_errors;

public:
	CResult() {}

	CResult(E* pcError)
	{
		if (pcError != NULL)
		{
			v_errors.push_back(pcError);
		}
	}

	CResult(std::vector<E*>& vErrors)
	{
		v_errors = vErrors;
		vErrors.clear();
	}

	CResult(const CResult<void, E>& cOther)
	{
		for (int i = 0; i < cOther.v_errors.size(); i++)
		{
			if (cOther.v_errors[i] != NULL)
			{
				v_errors.push_back(new E(*cOther.v_errors[i]));
			}
		}
	}

	~CResult()
	{
		for (int i = 0; i < v_errors.size(); i++)
		{
			if (v_errors[i] != NULL)
			{
				delete v_errors[i];
			}
		}
		v_errors.clear();
	}

	static CResult<void, E> cOk()
	{
		return CResult<void, E>();
	}
	static CResult<void, E> cFail(E* pcError)
	{
		return CResult<void, E>(pcError);
	}
	static CResult<void, E> cFail(std::vector<E*>& vErrors)
	{
		return CResult<void, E>(vErrors);
	}

	CResult<void, E>& operator=(const CResult<void, E>& cOther)
	{
		if (this == &cOther)
		{
			return *this;
		}

		for (size_t i = 0; i < v_errors.size(); i++) delete v_errors[i];
		v_errors.clear();

		for (size_t i = 0; i < cOther.v_errors.size(); i++)
		{
			if (cOther.v_errors[i] != NULL)
			{
				v_errors.push_back(new E(*cOther.v_errors[i]));
			}
		}

		return *this;
	}

	bool bIsSuccess() const
	{
		return (v_errors.empty());
	}

	const std::vector<E*>& vGetErrors() const
	{
		return v_errors;
	}
};



