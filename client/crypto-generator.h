#ifndef CRYPTOGENERATOR_H
#define CRYPTOGENERATOR_H

#ifdef CRYPTO2
    #include <openssl/pem.h>
    #include <openssl/x509.h>
    #include <iostream>
    EVP_PKEY * generate_key()
    {
        /* Allocate memory for the EVP_PKEY structure. */
        EVP_PKEY * pkey = EVP_PKEY_new();
        if(!pkey)
        {
            std::cerr << "Unable to create EVP_PKEY structure." << std::endl;
            return NULL;
        }

        /* Generate the RSA key and assign it to pkey. */
        auto primeNumber = BN_new();
        if (primeNumber == NULL){
            std::cerr << "Prime number not generated";
            EVP_PKEY_free(pkey);
            return NULL;
        }
        RSA* rsa = RSA_new();
        if (rsa == NULL){
            EVP_PKEY_free(pkey);
            BN_free(primeNumber);
            std::cerr << "No memory allocated for RSA";
            return NULL;
        }
        if (!BN_set_word(primeNumber, RSA_F4) || !RSA_generate_key_ex(rsa, 2048, primeNumber, NULL)){
            std::cerr << "Unable to generate 2048-bit RSA key." << std::endl;
            EVP_PKEY_free(pkey);
            BN_free(primeNumber);
            return NULL;
        }
        if(!EVP_PKEY_assign_RSA(pkey, rsa))
        {
            std::cerr << "Unable to assign 2048-bit RSA key." << std::endl;
            EVP_PKEY_free(pkey);
            BN_free(primeNumber);
            return NULL;
        }

        /* The key has been generated, return it. */
        BN_free(primeNumber);
        return pkey;
    }

    /* Generates a self-signed x509 certificate. */
    X509 * generate_x509(EVP_PKEY * pkey)
    {
        /* Allocate memory for the X509 structure. */
        X509 * x509 = X509_new();
        if(!x509)
        {
            std::cerr << "Unable to create X509 structure." << std::endl;
            return NULL;
        }

        /* Set the serial number. */
        ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);

        /* This certificate is valid from now until exactly one year from now. */
        X509_gmtime_adj(X509_get_notBefore(x509), 0);
        X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);

        /* Set the public key for our certificate. */
        X509_set_pubkey(x509, pkey);

        /* We want to copy the subject name to the issuer name. */
        X509_NAME * name = X509_get_subject_name(x509);

        /* Set the country code and common name. */
        X509_NAME_add_entry_by_txt(name, "C",  MBSTRING_ASC, (unsigned char *)"CA",        -1, -1, 0);
        X509_NAME_add_entry_by_txt(name, "O",  MBSTRING_ASC, (unsigned char *)"MyCompany", -1, -1, 0);
        X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)"localhost", -1, -1, 0);

        /* Now set the issuer name. */
        X509_set_issuer_name(x509, name);

        /* Actually sign the certificate with our key. */
        if(!X509_sign(x509, pkey, EVP_sha1()))
        {
            std::cerr << "Error signing certificate." << std::endl;
            X509_free(x509);
            return NULL;
        }

        return x509;
    }

    bool write_to_disk(EVP_PKEY * pkey, X509 * x509)
    {
        FILE * pkey_file = fopen("key.pem", "wb");
        if(!pkey_file)
        {
            return false;
        }
        bool ret = PEM_write_PrivateKey(pkey_file, pkey, NULL, NULL, 0, NULL, NULL);
        fclose(pkey_file);
        if(!ret)
        {
            return false;
        }
        FILE * x509_file = fopen("cert.pem", "wb");
        if(!x509_file)
        {
            return false;
        }
        ret = PEM_write_X509(x509_file, x509);
        fclose(x509_file);
        if(!ret)
        {
            return false;
        }
        return true;
    }
#endif // CRYPTO

#endif // CRYPTOGENERATOR_H
