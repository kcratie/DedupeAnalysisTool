#pragma once

#include <stdexcept>
#include <array>

#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>

#include <boost/array.hpp>
#include <boost/utility.hpp>
#include <boost/asio/buffer.hpp>

#if OPENSSL_VERSION_NUMBER < 0x10000000L
#error OpenSSL 1.0.0 or later is required for PKCS5_PBKDF2_HMAC and EVP aes-128-gcm functions.
#endif

namespace contentgraph {

typedef unsigned char byte;

namespace internal {
/// Check return values from OpenSSL and throw an exception if it failed.
/// @param what the logical operation being performed
/// @success the return value from OpenSSL API
inline void i_api(const char *what, int ret) {
	if (ret == 0) {
		std::string message(what);
		message += " failed.";
		throw std::runtime_error(message);
	}
}

/// Internal representation of a raw buffer. Uses boost::asio::buffer to translate the passed
/// container to a raw pointer and length pair.
template<typename T> struct raw {
	T ptr;
	int len;
	template<typename B> raw(const B &b) :
			ptr(boost::asio::buffer_cast<T>(b)), len(
					static_cast<int>(boost::asio::buffer_size(b))) {
	}
};
}

/// Remove sensitive data from the buffer
template<typename C> void cleanse(C &c) {
	internal::raw<void *> r(boost::asio::buffer(c));
	OPENSSL_cleanse(r.ptr, r.len);
}

/// A convenience typedef for a 128 bit block.
typedef std::array<unsigned char, 16> block;

/// Checks if the  underlying PRNG is sufficiently seeded. In  the (exceptional) situation where
/// this check  returns 'false', you  /must/ use the  OpenSSL seed routines  RAND_seed, RAND_add
/// directly to add entropy to the underlying PRNG.
//bool i_prng_ok() {
//	return RAND_status() == 1;
//}

/// Fills the passed container with random bytes.
/// @param c  (output) container populated with random bits
template<typename C> void fill_random(C &c) {
	internal::raw<unsigned char *> r(boost::asio::buffer(c));
	internal::i_api("random bytes", RAND_bytes(r.ptr, r.len));
}

/// Derives a  key from a  password and salt  using PBKDF2 with  HMAC-SHA256 as the  chosen PRF.
/// Although the routine can generate arbitrary length  keys, it is best to use crypto::block as
/// the type for the key  parameter, since it fixes the key length to 128  bit which is what the
/// other primitives in the wrapper (crypto::hash, crypto::cipher) require.
/// @param key      (output) container populated with the key bits
/// @param pass     (input)  container holding the user password
/// @param salt     (input)  container holding the salt bytes
/// @param c        (input)  PBKDF2 iteration count (default=10000)
template<typename C1, typename C2, typename C3>
void derive_key(C3 &key, const C1 &pass, const C2 &salt, int c = 10000) {
	internal::raw<const char *> p(boost::asio::buffer(pass));
	internal::raw<unsigned char *> k(boost::asio::buffer(key));
	internal::raw<const unsigned char *> s(boost::asio::buffer(salt));
	internal::i_api("key derivation",
			PKCS5_PBKDF2_HMAC(p.ptr, p.len, s.ptr, s.len, c, EVP_sha256(),
					k.len, k.ptr));
}

/// Generates a keyed or a plain cryptographic hash.
class Digest: boost::noncopyable {
public:
	static const size_t SHA256_DIGEST_LENGTH = 32;
	/// A convenience typedef for a 256 bit SHA-256 value.
	typedef std::array<byte, SHA256_DIGEST_LENGTH> value_type;

	/// The plain hash constructor. Initializes the underlying hash context.
	Digest() :
			keyed_(false), digest_(EVP_sha256()), md_context_(
					EVP_MD_CTX_create()) {
		internal::i_api("digest initialization",
				EVP_DigestInit_ex(md_context_, digest_, NULL));
	}

	/// The keyed hash constructor. Initializes the underlying hash context.
	/// @param key (input) container holding the secret key
	template<typename C>
	Digest(const C &key) :
			keyed_(true), digest_(EVP_sha256()) {
		HMAC_CTX_init(&hmac_context_);
		internal::raw<const void *> k(boost::asio::buffer(key));
		internal::i_api("mac initialization",
				HMAC_Init_ex(&hmac_context_, k.ptr, k.len, digest_, NULL));
	}

	/// Add the  contents of the passed  container to the  underlying context. This method  can be
	/// invoked multiple times to add all the data that needs to be hashed.
	/// @param data (input) container holding the input data
	Digest &update(const unsigned char * data, size_t len) {
		internal::i_api("add data to hash",
				(keyed_ ?
						HMAC_Update(&hmac_context_,
								(const unsigned char *) data, len) :
						EVP_DigestUpdate(md_context_, data, len)));
		return *this;
	}

	/// Get the  resultant hash value. This  method also reinitializes the  underlying context, so
	/// the same instance can be reused to compute more hashes.
	/// @param sha (output) container populated with the hash/mac bits
	void finalize(value_type &sha) {
		//internal::raw<unsigned char *> d(boost::asio::buffer(sha));
		internal::i_api("finalization of hash",
				(keyed_ ?
						HMAC_Final(&hmac_context_, sha.data(), 0) :
						EVP_DigestFinal_ex(md_context_, sha.data(), NULL)));
		internal::i_api("reinitialization of hash",
				(keyed_ ?
						HMAC_Init_ex(&hmac_context_, NULL, 0, NULL, NULL) :
						EVP_DigestInit_ex(md_context_, digest_, NULL)));
	}

	/// Cleans up the underlying context.
	~Digest() {
		keyed_ ?
				HMAC_CTX_cleanup(&hmac_context_) :
				EVP_MD_CTX_destroy(md_context_);
	}
private:
	bool keyed_;
	const EVP_MD *digest_;
	HMAC_CTX hmac_context_;
	EVP_MD_CTX *md_context_;
};

}
