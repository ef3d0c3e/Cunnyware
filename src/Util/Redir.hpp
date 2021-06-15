#ifndef UTIL_REDIR_HPP
#define UTIL_REDIR_HPP

#include <fmt/format.h>
#include <stdio.h>
#include <unistd.h>

////////////////////////////////////////////////
/// \brief Redirection utility
////////////////////////////////////////////////
class Redir
{
	FILE* m_file;
	int m_fd;
	int m_rfd;
#if __x86_64__
	using fpos = fpos64_t;
#else
	using fpos = fpos_t;
#endif
	fpos m_pos;

public:
	constexpr Redir() {}
	constexpr ~Redir() {}

	void Create(FILE* file, const std::string& path)
	{
		m_file = file;
		const int fd = fileno(file);
		if (fd == -1)
			fmt::print("Redir::Create({}), {} `fileno` returned -1", (const void*)file, path);

#if __x86_64__
		fgetpos64(file, &m_pos);
		m_fd = dup(fd);
		FILE* rfile = freopen64(path.c_str(), "w", file);
#else
		fgetpos(file, &m_pos);
		m_fd = dup(fd);
		FILE* rfile = freopen(path.c_str(), "w", file);
#endif
		m_rfd = fileno(rfile);
		if (m_rfd == -1)
			fmt::print("Redir::Create({}), {} `fileno` returned -1", (const void*)file, path);
	}

	void Release()
	{
		int fd = fileno(stdout);
		if (fd == -1)
			fmt::print("Redir::Release() `fileno({})` returned -1", (const void*)m_file);
		fsync(m_rfd);
		dup2(m_fd, fd);
		close(m_fd);
		close(m_rfd);
		clearerr(m_file);
#if __x86_64__
		fsetpos64(m_file, &m_pos);
#else
		fsetpos(m_file, &m_pos);
#endif
	}
};

#endif // UTIL_REDIR_HPP
