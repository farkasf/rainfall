#include <unistd.h>
#include <cstring>

class	N
{
	private:
		int		_num;
		char	_annotation[100];

	public:
		N(int input);

		int 	operator+(N const &next);
		int 	operator-(N const &next);
		void	setAnnotation(char const *annot);
};

N::N(int input) {
	this->_num = input;
}

int	N::operator+(N const &next) {
	return (this->_num + next._num);
}

int	N::operator-(N const &next)
{
	return (this->_num - next._num);
}

void	N::setAnnotation(char const *annot) {
	memcpy(this->_annotation, annot, strlen(annot) + 1);
}

int	main(int ac, char **av)
{
	if (ac < 2)
		_exit(1);

	N	*five = new N(5);
	N	*six = new N(6);

	five->setAnnotation(av[1]);
	return (*five + *six);
}
