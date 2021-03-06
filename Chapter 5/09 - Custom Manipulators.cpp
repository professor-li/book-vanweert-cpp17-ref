#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

class Person {
public:
	Person() = default;
	explicit Person(std::string first, std::string last = "",
		bool isVIP = false)
		: m_first(move(first)), m_last(move(last)), m_isVIP(isVIP) {}

	const std::string& GetFirstName() const { return m_first; }
	void SetFirstName(std::string first) { m_first = move(first); }

	const std::string& GetLastName() const { return m_last; }
	void SetLastName(std::string last) { m_last = move(last); }

	bool IsVIP() const { return m_isVIP; }

private:
	std::string m_first, m_last;
	bool m_isVIP = false;
};

// Comparison operator
bool operator<(const Person& lhs, const Person& rhs) {
	if (lhs.IsVIP() != rhs.IsVIP()) return rhs.IsVIP();
	if (lhs.GetLastName() != rhs.GetLastName())
		return lhs.GetLastName() < rhs.GetLastName();
	return lhs.GetFirstName() < rhs.GetFirstName();
}

// Equality operator
bool operator==(const Person & lhs, const Person & rhs) {
	return lhs.IsVIP() == rhs.IsVIP()
		&& lhs.GetFirstName() == rhs.GetFirstName()
		&& lhs.GetLastName() == rhs.GetLastName();
}

const int dot_xalloc = std::ios_base::xalloc(); // global constant

// I/O manipulators that toggle the 'dot' field of a stream
std::ios_base& dot(std::ios_base& stream) {
	stream.iword(dot_xalloc) = 1; return stream;
}
std::ios_base& nodot(std::ios_base& stream) {
	stream.iword(dot_xalloc) = 0; return stream;
}

// Output stream operator that 'dots' the first name if requested
std::ostream& operator<<(std::ostream& out, const Person& person) {
	if (out.iword(dot_xalloc))
		out << person.GetFirstName().front() << '.';
	else
		out << person.GetFirstName();
	return out << ' ' << person.GetLastName();
}

std::istream& operator>>(std::istream& in, Person& person) {
	std::string firstName, lastName;
	in >> std::quoted(firstName) >> std::quoted(lastName);
	if (firstName.empty())             // fail if invalid data is read
		in.setstate(std::ios::failbit);  // add fail bit
	else if (in)                       // only if reading succeeded
		person = Person(std::move(firstName), std::move(lastName));
	return in;
}

int main()
{
	Person person("Hubert", "Gruber");
	std::cout << person << '\n' << dot << person << std::endl;

	std::cout << std::endl;
}
