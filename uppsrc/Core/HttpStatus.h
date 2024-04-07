namespace HttpStatus {
	constexpr int CONTINUE            = 100; // RFC 9110, 15.2.1
	constexpr int SWITCHING_PROTOCOLS = 101; // RFC 9110, 15.2.2
	constexpr int PROCESSING          = 102; // RFC 2518, 10.1
	constexpr int EARLY_HINTS         = 103; // RFC 8297
	
	constexpr int OK                     = 200; // RFC 9110, 15.3.1
	constexpr int CREATED                = 201; // RFC 9110, 15.3.2
	constexpr int ACCEPTED               = 202; // RFC 9110, 15.3.3
	constexpr int NON_AUTHORITATIVE_INFO = 203; // RFC 9110, 15.3.4
	constexpr int NO_CONTENT             = 204; // RFC 9110, 15.3.5
	constexpr int RESET_CONTENT          = 205; // RFC 9110, 15.3.6
	constexpr int PARTIAL_CONTENT        = 206; // RFC 9110, 15.3.7
	constexpr int MULTI_STATUS           = 207; // RFC 4918, 11.1
	constexpr int ALREADY_REPORTED       = 208; // RFC 5842, 7.1
	constexpr int IM_USED                = 226; // RFC 3229, 10.4.1
	
	constexpr int MULTIPLE_CHOICES   = 300; // RFC 9110, 15.4.1
	constexpr int MOVED_PERMANENTLY  = 301; // RFC 9110, 15.4.2
	constexpr int FOUND              = 302; // RFC 9110, 15.4.3
	constexpr int SEE_OTHER          = 303; // RFC 9110, 15.4.4
	constexpr int NOT_MODIFIED       = 304; // RFC 9110, 15.4.5
	constexpr int USE_PROXY          = 305; // RFC 9110, 15.4.6
	constexpr int _                  = 306; // RFC 9110, 15.4.7 (Unused)
	constexpr int TEMPORARY_REDIRECT = 307; // RFC 9110, 15.4.8
	constexpr int PERMANENT_REDIRECT = 308; // RFC 9110, 15.4.9
	
	constexpr int BAD_REQUEST                     = 400; // RFC 9110, 15.5.1
	constexpr int UNAUTHORIZED                    = 401; // RFC 9110, 15.5.2
	constexpr int PAYMENT_REQUIRED                = 402; // RFC 9110, 15.5.3
	constexpr int FORBIDDEN                       = 403; // RFC 9110, 15.5.4
	constexpr int NOT_FOUND                       = 404; // RFC 9110, 15.5.5
	constexpr int METHOD_NOT_ALLOWED              = 405; // RFC 9110, 15.5.6
	constexpr int NOT_ACCEPTABLE                  = 406; // RFC 9110, 15.5.7
	constexpr int PROXY_AUTH_REQUIRED             = 407; // RFC 9110, 15.5.8
	constexpr int REQUEST_TIEMOUT                 = 408; // RFC 9110, 15.5.9
	constexpr int CONFLICT                        = 409; // RFC 9110, 15.5.10
	constexpr int GONE                            = 410; // RFC 9110, 15.5.11
	constexpr int LENGTH_REQUIRED                 = 411; // RFC 9110, 15.5.12
	constexpr int PRECONDITION_FAILED             = 412; // RFC 9110, 15.5.13
	constexpr int REQUEST_ENTITY_TOO_LARGE        = 413; // RFC 9110, 15.5.14
	constexpr int REQUEST_URI_TOO_LONG            = 414; // RFC 9110, 15.5.15
	constexpr int UNSUPPORTED_MEDIA_TYPE          = 415; // RFC 9110, 15.5.16
	constexpr int REQUEST_RANGE_NOT_SATISFIABLE   = 416; // RFC 9110, 15.5.17
	constexpr int EXPECTATION_FAILED              = 417; // RFC 9110, 15.5.18
	constexpr int TEAPOT                          = 418; // RFC 9110, 15.5.19 (Unused)
	constexpr int MISDIRECTED_REQUEST             = 421; // RFC 9110, 15.5.20
	constexpr int UNPROCESSABLE_ENTITY            = 422; // RFC 9110, 15.5.21
	constexpr int LOCKED                          = 423; // RFC 4918, 11.3
	constexpr int FAILED_DEPENDENCY               = 424; // RFC 4918, 11.4
	constexpr int TOO_EARLY                       = 425; // RFC 8470, 5.2.
	constexpr int UPGRADE_REQUIRED                = 426; // RFC 9110, 15.5.22
	constexpr int PRECONDITION_REQUIRED           = 428; // RFC 6585, 3
	constexpr int TOO_MANY_REQUESTS               = 429; // RFC 6585, 4
	constexpr int REQUEST_HEADER_FIELDS_TOO_LARGE = 431; // RFC 6585, 5
	constexpr int UNAVAILABLE_FOR_LEGAL_REASONS   = 451; // RFC 7725, 3

	constexpr int INTERNAL_SERVER_ERROR           = 500; // RFC 9110, 15.6.1
	constexpr int NOT_IMPLEMENTED                 = 501; // RFC 9110, 15.6.2
	constexpr int BAD_GATEWAY                     = 502; // RFC 9110, 15.6.3
	constexpr int SERVICE_UNAVAILABLE             = 503; // RFC 9110, 15.6.4
	constexpr int GATEWAY_TIMEOUT                 = 504; // RFC 9110, 15.6.5
	constexpr int HTTP_VERSION_NOT_SUPPORTED      = 505; // RFC 9110, 15.6.6
	constexpr int VARIANT_ALSO_NEGOTIATES         = 506; // RFC 2295, 8.1
	constexpr int INSUFFICIENT_STORAGE            = 507; // RFC 4918, 11.5
	constexpr int LOOP_DETECTED                   = 508; // RFC 5842, 7.2
	constexpr int NOT_EXTENDED                    = 510; // RFC 2774, 7
	constexpr int NETWORK_AUTHENTICATION_REQUIRED = 511; // RFC 6585, 6

	String ToString(int status);
}

class HttpStatusLine final {
public:
	HttpStatusLine(int code)
		: m_code(code)
		, m_phrase(HttpStatus::ToString(code))
	{}
	
	HttpStatusLine(int code, const String& phrase)
		: m_code(code)
		, m_phrase(phrase)
	{}
	
	int GetCode() const      { return m_code; }
	String GetPhrase() const { return m_phrase; }
	
private:
	int m_code;
	String m_phrase;
};
