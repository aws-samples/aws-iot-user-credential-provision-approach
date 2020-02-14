#ifndef AWS_CLIENT_CREDENTIAL_KEYS_H
#define AWS_CLIENT_CREDENTIAL_KEYS_H

#include <stdint.h>

/*
 * PEM-encoded client certificate.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----"
 */

/* This is default client certificate, it's invalid but used to pass TLS Connect validation*/
#define keyCLIENT_CERTIFICATE_PEM \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDWjCCAkKgAwIBAgIVAOTXUSRQWX05l+abaCELTwe6vhfEMA0GCSqGSIb3DQEB\n"\
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"\
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xOTExMDYwNzE2\n"\
"NTRaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"\
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCr3lK08aspW1UjvVcS\n"\
"B6pV9s66DEdHpA1934Du+RTE3fClqqDDYZkvEV1t+jRElFEEp/bFsmYb3FrskOJj\n"\
"dpBMB/rZ/auXJo3+IgGYNiwmEY0f9IMj9zIrNK1sgO0VdbBrNiYLarfCMpeYkmuL\n"\
"d+GLj1CWyWBQSzA8QBrcygInzjZoAKRWIJl8PXKahgb2zsdbHEG9Sa9wjPWhzItr\n"\
"gJBJndz2dgQ0gp9hXUW0CafSbvWLruuXzlJOXl7RawkKl3XabLACMPQ9hLw0kCy8\n"\
"T1hzKETOdUKEKiKDUNABPOxsfsugaenPxVeMR/C00ydTxU0TApTXJ6V9Sv6FTBWx\n"\
"klPlAgMBAAGjYDBeMB8GA1UdIwQYMBaAFGrzv/GVJulRXO6ma5DtVwLUuwouMB0G\n"\
"A1UdDgQWBBRpO2os2FYysQBgxVfkpf/H8MMV/zAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"\
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAQ+vndRMmjELWaiXj8nZUkJws\n"\
"JFThCYkghZkqE8eZaAzuHDhDEMlcfzxuRhsBdEimwXfteW+lE0OhIHhdk6bm6eLK\n"\
"HJG4Lh17ao8duhf/QBVFkFVXJXTfChRfmlrA+If5EIrTQxIkrXwNOzfDT30DXcgQ\n"\
"zxPU34ErECGTxh2qaftfQ2tbZZFKxh2qLbNt7uHdEfdaHaN2I8Wa+ZI8M7dlRR3U\n"\
"mJAwY/cFMMMKXFYbGE+3je2waSnN90hAIPf5KIV2oLfxJ2CFiH3wieyrDkHdoPAu\n"\
"jjYEjoAr15MowYqtVLv9GpXfoTCsVfMx6mG290czCyE8sNBa0q1mUb3+bNRgYQ==\n"\
"-----END CERTIFICATE-----"

/*
 * PEM-encoded client private key.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN RSA PRIVATE KEY-----\n"\
 * "...base64 data...\n"\
 * "-----END RSA PRIVATE KEY-----"
 */

/* This is default client certificate, it's invalid but used to pass TLS Connect validation*/
#define keyCLIENT_PRIVATE_KEY_PEM \
"-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIEogIBAAKCAQEAq95StPGrKVtVI71XEgeqVfbOugxHR6QNfd+A7vkUxN3wpaqg\n"\
"w2GZLxFdbfo0RJRRBKf2xbJmG9xa7JDiY3aQTAf62f2rlyaN/iIBmDYsJhGNH/SD\n"\
"I/cyKzStbIDtFXWwazYmC2q3wjKXmJJri3fhi49QlslgUEswPEAa3MoCJ842aACk\n"\
"ViCZfD1ymoYG9s7HWxxBvUmvcIz1ocyLa4CQSZ3c9nYENIKfYV1FtAmn0m71i67r\n"\
"l85STl5e0WsJCpd12mywAjD0PYS8NJAsvE9YcyhEznVChCoig1DQATzsbH7LoGnp\n"\
"z8VXjEfwtNMnU8VNEwKU1yelfUr+hUwVsZJT5QIDAQABAoIBABFiYpFE5KBBScJc\n"\
"s4LyXYtmdJCjD06yzF6QDfWeIjirjlzjfnFk3N7C7eVi6oiJTAyhhLeIjQWCa3wE\n"\
"R7Hx2zSllf0YyJN4ZeJvxCOGTz7mhZTW4bDwnxV5tZUFEXYbOmYNyxlaiQ3OmkDn\n"\
"hpNL4/d+3pLx+ZkFkfJ2NYFhuYgXDD/1kL1Dgl8AVeP6DNhTMrdA8/8aVLnkycpI\n"\
"bQAtkUAhm2GuYiRSQzX60W4EqFy5uZ8axuR3mQnoDxytM7gawMT0GgF0QNjcAHn1\n"\
"kBk+OOArHSRHUC6oeuFLL78WxPSOFTEKjOj+GiOPSxhRwVv6IuzrnYELjRaXoTlK\n"\
"7aSwUaECgYEA1BTJl4xylkEO3VGRLWGi2Rq4tzlihfB/tMJFEE30t781AGwKsqSZ\n"\
"fxfIM7sj97WGqgto0fmDB1yBTNVrnSK6qZJF8S/ISoZSofGR/fJI/P7Hc3SAJl+f\n"\
"WGstoeu+JYQ2yV/H92EEOBR2EjFVJU6S9xD+73l6Pcr4nuqAei1oZ20CgYEAz3W1\n"\
"TfF8SVeT4gHc1hezDdzERR2+xipgcv8wE7YQ8ju/9t3/gOF+yijVPOIl0Wz1pLc2\n"\
"TisTvYFnUppR+M/I/eEB0/aVhpGWYY1onL4eN/zuukf7gLSjwS6mNcIjImGeQMVB\n"\
"58qqS2PWZp59+0+iI6+ozLE+uF1Ln1Pi73LJe1kCgYAeM7ultl2XIjqwr+4BzXa4\n"\
"fkKx7R0nv+Ef586YDNH93N/6aGh5mlE1tThQjaUIoLRFwQwzYtoTEUxkmbwSi6CT\n"\
"h34oY8qdOlXINDO7O2bDiLD0szGQxWtW1ySPe5f/lb2LnfVHOGy2Y8uQua/LeU4a\n"\
"mzQC/IS5Cg+v6etT+uV0XQKBgDgaUeR5KK5A08orzMfM8uwNxroWODAo55pRrnO2\n"\
"KdEIHXqqQF1fHcWX00q6sZV4DNn1fecXuSfnZcSt3PsXXfhiL/ddWMRifLN25VJ2\n"\
"C4ACk6CmtLnyn9IvRl2FWbEDiyNi9Na1NOZcYm+v7svWljdmWpS4XgjHmhrFObqd\n"\
"ZSVJAoGAFIcznhrvudDj/b9oYqYLaqGZJUGs9xD0utIDlo/aAiGcCCEEqxNWuK4Z\n"\
"POBjPx+ezulNyCMgINdHDstiz/pDm609vd/LhwgSqlw7b1yk/HFF+TzkP4t8XhQr\n"\
"DAezM3WPxRUYnUfKi8JVq32b2v6Gg7zFhXM3zrnsGeaw1uE5G7s=\n"\
"-----END RSA PRIVATE KEY-----"

/*
 * PEM-encoded Just-in-Time Registration (JITR) certificate (optional).
 *
 * If used, must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----"
 */
#define keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM  NULL

/* The constants above are set to const char * pointers defined in aws_dev_mode_key_provisioning.c,
 * and externed here for use in C files.  NOTE!  THIS IS DONE FOR CONVENIENCE
 * DURING AN EVALUATION PHASE AND IS NOT GOOD PRACTICE FOR PRODUCTION SYSTEMS
 * WHICH MUST STORE KEYS SECURELY. */
extern const char clientcredentialCLIENT_CERTIFICATE_PEM[];
extern const char* clientcredentialJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM;
extern const char clientcredentialCLIENT_PRIVATE_KEY_PEM[];
extern const uint32_t clientcredentialCLIENT_CERTIFICATE_LENGTH;
extern const uint32_t clientcredentialCLIENT_PRIVATE_KEY_LENGTH;

#endif /* AWS_CLIENT_CREDENTIAL_KEYS_H */
