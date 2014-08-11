#ifndef DISSENT_CRYPTO_DIFFIE_HELLMAN_KEY_H_GUARD
#define DISSENT_CRYPTO_DIFFIE_HELLMAN_KEY_H_GUARD

#include <QByteArray>
#include <QSharedData>
#include <QString>

#include "Integer.hpp"

namespace Dissent {
namespace Crypto {
  class IDiffieHellmanImpl : public QSharedData
  {
    public:
      virtual ~IDiffieHellmanImpl() {}
      virtual QByteArray GetPublicComponent() const = 0;
      virtual QByteArray GetPrivateComponent() const = 0;
      virtual QByteArray GetSharedSecret(const QByteArray &remote_pub) const = 0;
  };

  /**
   * Stores a Diffie-Hellman exchange -- shared secret exchanged in plaintext
   */
  class DiffieHellman {
    public:
      static QByteArray GetP()
      {
        static QByteArray p = QByteArray::fromHex("0x"
          "B10B8F96 A080E01D DE92DE5E AE5D54EC 52C99FBC FB06A3C6"
          "9A6A9DCA 52D23B61 6073E286 75A23D18 9838EF1E 2EE652C0"
          "13ECB4AE A9061123 24975C3C D49B83BF ACCBDD7D 90C4BD70"
          "98488E9C 219A7372 4EFFD6FA E5644738 FAA31A4F F55BCCC0"
          "A151AF5F 0DC8B4BD 45BF37DF 365C1A65 E68CFDA7 6D4DA708"
          "DF1FB2BC 2E4A4371");
        return p;
      }

      static Integer GetPInt()
      {
        static Integer p(GetP());
        return p;
      }

      static QByteArray GetG()
      {
        static QByteArray g = QByteArray::fromHex("0x"
          "A4D1CBD5 C3FD3412 6765A442 EFB99905 F8104DD2 58AC507F"
          "D6406CFF 14266D31 266FEA1E 5C41564B 777E690F 5504F213"
          "160217B4 B01B886A 5E91547F 9E2749F4 D7FBD7D3 B9A92EE1"
          "909D0D22 63F80A76 A6A24C08 7A091F53 1DBF0A01 69B6A28A"
          "D662A4D1 8E73AFA3 2D779D59 18D08BC8 858F4DCE F97C2A24"
          "855E6EEB 22B3B2E5");
        return g;
      }

      static Integer GetGInt()
      {
        static Integer g(GetG());
        return g;
      }

      static QByteArray GetQ()
      {
        static QByteArray q = QByteArray::fromHex("0x"
          "F518AA87 81A8DF27 8ABA4E7D 64B7CB9D 49462353"); 
        return q;
      }

      static Integer GetQInt()
      {
        static Integer q(GetQ());
        return q;
      }

      /**
       * Constructor
       * @param data empty, private key, or seed if seed is true
       * @param seed specifies is data is a private key or a (default) seed
       */
      DiffieHellman(const QByteArray &data = QByteArray(),
          bool seed = true);

      /**
       * Retrieves the public component of the Diffie-Hellman agreement
       */
      QByteArray GetPublicComponent() const
      {
        return m_data->GetPublicComponent();
      }

      /**
       * Retrieves the private component of the Diffie-Hellman agreement
       */
      QByteArray GetPrivateComponent() const
      {
        return m_data->GetPrivateComponent();
      }

      /**
       * Return the shared secret given the other sides public component
       * @param remote_pub the other sides public component
       */
      QByteArray GetSharedSecret(const QByteArray &remote_pub) const
      {
        return m_data->GetSharedSecret(remote_pub);
      }

      /**
       * Return a non-interactive zero-knowledge proof of a shared Diffie-Hellman
       * secret.
       * A wants to prove to C that the shared secret between A and B was correctly
       * generated by A. A then calls this method as:
       * 
       *    private_key_A->ProveSharedSecret(public_key_B)
       *
       * and sends the output to C
       */
      QByteArray ProveSharedSecret(const QByteArray &remote_pub) const;

      /**
       * Verify a non-interactive zero-knowledge proof of a shared Diffie-Hellman secret.
       * A wants to prove to C that the shared secret between A and B was correctly
       * generated by A. The verifier C then calls this method as:
       * 
       *    public_key_A->VerifySharedSecret(public_key_B)
       *
       * @returns QByteArray() if verification fails, otherwise returns the shared secret
       */
      static QByteArray VerifySharedSecret(const QByteArray &prover_pub,
          const QByteArray &remote_pub, const QByteArray &proof);

    private:
      QSharedDataPointer<IDiffieHellmanImpl> m_data;
  };
}
}

#endif