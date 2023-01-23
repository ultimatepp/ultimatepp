#include <Core/SSL/RSA.h>

using namespace Upp;

// 2048 bit pkcs8
const String PrivateKey =
R"(-----BEGIN PRIVATE KEY-----
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDD15JaB0iT9YgP
9FYzLFtg2f/MhLmpfijFWngGXGLWm6X0s70ce63Hnia+jh7gZ3e5g/k3tejZcIL5
0EBp4iS6CLgoxLD7F6c6qUYpEIjmP+54/nPzQ+gLJOvI3xIUY3vu7LZZg4MnFm1/
OCFdc6tjrZU2z3GlL/3PikEST3RdLr5D5AC6ihVFJB+NJc0FjjCSEvRbeesCwdCn
uRXTa6woQan+k5tdVXSRo/BoeNlpXLWLMcZZyCL0cBq1SQQ23Eozrza/MSScZ2sH
+CSI9Zy4BpNc/3IMAPj7XltNa0O98606Uhhb3TSb1DzraNjbWQboolXsVYyF/cUa
9SEC+215AgMBAAECggEALxcyRziSIJW9Ak0XvxVO4aJ5qiVN+W3vAslmq+ziDUV+
6bCga+1nU2T5Xv+QeBgZB0AvGFDC7SnrLIOBQZSRjTQUiCUHg1DkMqxKRQyHRoof
3vMcUWXGMYvOsE3HLKThQ4mirTJ8wlTXvmvyReTO1NiD1RqbcoNwLcvKKst1KQRs
mxmcfz2D/GLvM+XOElP5YPZHvcUru9S6o7RMmixsuXU5OJ9fMAObfNsmP2lmWcCC
lgKXVHzwOh/DAxddc8/1aWev927p2b6L8G1WL8HjRiwE0ZO2nZBpTM1ZeM2c2XO2
+iuvcY9tI/RxL9TA1Wy5HazSQR50+AWAUXUV3MViqwKBgQDKW4WxZdL4nPh5P/dd
N0KfcoK+oiVYzhvhvtMQKAQ4JpLvhEfmcA3CG9y2L+RBzaL44Bk1Yjqq1Pttwekl
Sz1yR7K56e0XBwZ5vnyfjKGM3EUkQvuQrJZEB6kX4SEeaAk0kW5/dgVijpf6XYB/
yUDGf1NfreU0JH2+v5k7iJU3CwKBgQD3weVvlpK4EXcO8pssq556ogJ7wZWXrBE3
jzOe2JLOHMvvNDqJ5Ss+XUUmgag2UDZEZ6BzrPVakmQHuR8HAKa+O3TJEeDtqLyL
1hBhak6isqRx0CGnmavzJK1CFqKr7RF+UrKPK8wIBz9oxEgBT6vWrl4pEWdaIry8
Wj9r5sAwCwKBgQDBTe6HyTpg7ipaQWW+sUQ+ZrhI/UHp9HIkExMyktd9ADRLA7Vf
h1lNPSp5C4Tj61msnv7VxTE/8dcBQUTDsH1RRvaPrMwkvDxOXk5UZ5YHpn+SJT6F
NKkS8bJxMkld74ojom3I6ug3/vllXJ6c85aiSA1QECEsz1MjvYnWmiHYgwKBgAr2
lYR+kTd77lzG937eiUC78ogvZMM1Zf2jZJFe3AAq3gtTsFNxxJwZ3Hl7u0nL+trE
CyGTQbgkSjDdTDYvHTHFx8Et/VoG4XD+o/LrlVc1uHkg0Jxt/GHh+Zi3D7Oz5dZF
KomTUo/4YiHsrZMBR4VUae4/Sud+KgPbqnrH1zFJAoGBAIWtYZVK1uDEUHwmFSF2
/7otttkRIES5n4Un3rjKoadqqtTZep5TEj0NZqKsvbJ5R1ah+DzqwlRWeZsKlljN
3g9nEiwyGSfzCDqpECfLCyDdI+mqbtaAybid9MxXZiYolVeV3etrjpu8ATdpVfQW
HVP7vTWKIHtSfPcJH/PENJbJ
-----END PRIVATE KEY-----)";

// pkcs1
const String PublicKey =
R"(-----BEGIN RSA PUBLIC KEY-----
MIIBCgKCAQEAw9eSWgdIk/WID/RWMyxbYNn/zIS5qX4oxVp4Blxi1pul9LO9HHut
x54mvo4e4Gd3uYP5N7Xo2XCC+dBAaeIkugi4KMSw+xenOqlGKRCI5j/ueP5z80Po
CyTryN8SFGN77uy2WYODJxZtfzghXXOrY62VNs9xpS/9z4pBEk90XS6+Q+QAuooV
RSQfjSXNBY4wkhL0W3nrAsHQp7kV02usKEGp/pObXVV0kaPwaHjZaVy1izHGWcgi
9HAatUkENtxKM682vzEknGdrB/gkiPWcuAaTXP9yDAD4+15bTWtDvfOtOlIYW900
m9Q862jY21kG6KJV7FWMhf3FGvUhAvtteQIDAQAB
-----END RSA PUBLIC KEY-----)";

CONSOLE_APP_MAIN
{
	// load keys
	RSAPrivateKey priv(PrivateKey);
	RSAPublicKey pub(PublicKey);

	// sign message
	String message = "This is a secret message";
	String signature = priv.SignRS256(message);

	// verify signature
	bool signOk = pub.VerifyRS256(message, signature);
	Cout() << "Signature is " << (signOk ? "valid\n" : "invalid\n");
	
	// also generate a key
	RSAPrivateKey privGen;
	privGen.Generate(2048);
	ASSERT(privGen);
	
	String sigGen = privGen.SignRS256(message);
	
	// verify
	RSAPublicKey pubGen = privGen.GetPublicKey();
	signOk = pubGen.VerifyRS256(message, sigGen);
	Cout() << "Signature is " << (signOk ? "valid\n" : "invalid\n");
}
