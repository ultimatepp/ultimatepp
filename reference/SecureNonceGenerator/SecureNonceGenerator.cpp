#include <Core/Core.h>
#include <Core/SSL/SSL.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
    // Initialize logging to console
    StdLogSetup(LOG_COUT);

    // Generate a 16-byte secure nonce
    String nonce = SecureNonce(16);

    if(!nonce.IsVoid()) {
        LOG("Generated nonce (hex): " << HexString(nonce));
        LOG("Nonce length: " << nonce.GetLength());
    }
    else {
        LOG("Failed to generate nonce.");
    }
}
