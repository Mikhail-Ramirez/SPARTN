# communications/encryption.py
import logging
import os
from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.primitives.ciphers.aead import AESGCM

# Global keys (generated at startup)
RSA_PRIVATE_KEY = None
RSA_PUBLIC_KEY = None
AES_KEY = None  # Will be established via handshake

def generate_rsa_keys():
    global RSA_PRIVATE_KEY, RSA_PUBLIC_KEY
    RSA_PRIVATE_KEY = rsa.generate_private_key(public_exponent=65537, key_size=2048)
    RSA_PUBLIC_KEY = RSA_PRIVATE_KEY.public_key()
    logging.info("[Encryption] Generated RSA key pair.")

def get_rsa_public_key_pem():
    return RSA_PUBLIC_KEY.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )

def rsa_encrypt(message: bytes, public_key):
    return public_key.encrypt(
         message,
         padding.OAEP(
             mgf=padding.MGF1(algorithm=hashes.SHA256()),
             algorithm=hashes.SHA256(),
             label=None
         )
    )

def rsa_decrypt(ciphertext: bytes):
    return RSA_PRIVATE_KEY.decrypt(
         ciphertext,
         padding.OAEP(
             mgf=padding.MGF1(algorithm=hashes.SHA256()),
             algorithm=hashes.SHA256(),
             label=None
         )
    )

def generate_aes_key():
    global AES_KEY
    AES_KEY = os.urandom(16)  # AES-128 key
    logging.info("[Encryption] Generated AES key.")

def aes_encrypt(message: bytes):
    aesgcm = AESGCM(AES_KEY)
    nonce = os.urandom(12)
    ct = aesgcm.encrypt(nonce, message, None)
    return nonce + ct

def aes_decrypt(ciphertext: bytes):
    aesgcm = AESGCM(AES_KEY)
    nonce = ciphertext[:12]
    ct = ciphertext[12:]
    return aesgcm.decrypt(nonce, ct, None)

# --- Handshake functions ---
def perform_handshake_send(sock):
    """
    Sender-side handshake:
      1. Send "handshake_start" with our RSA public key.
      2. Receive "handshake_response" and peer's RSA public key.
      3. Generate AES key, send it encrypted with peer's RSA public key as "handshake_key".
      4. Receive "handshake_ack" (must decrypt to "ack").
    """
    # Step 1: Send our public key
    pubkey_pem = get_rsa_public_key_pem()
    sock.sendall(b"handshake_start," + pubkey_pem + b"\n")
    logging.info("[Encryption] Sent handshake_start with our RSA public key.")

    # Step 2: Receive peer public key
    response = sock.recv(4096)
    if not response.startswith(b"handshake_response,"):
        raise Exception("Invalid handshake response")
    peer_pubkey_pem = response[len(b"handshake_response,"):].strip()
    logging.info("[Encryption] Received handshake_response with peer RSA public key.")
    peer_public_key = serialization.load_pem_public_key(peer_pubkey_pem)

    # Step 3: Generate AES key and send it encrypted
    generate_aes_key()
    encrypted_aes = rsa_encrypt(AES_KEY, peer_public_key)
    sock.sendall(b"handshake_key," + encrypted_aes + b"\n")
    logging.info("[Encryption] Sent handshake_key with encrypted AES key.")

    # Step 4: Receive ack and verify
    ack = sock.recv(4096)
    if not ack.startswith(b"handshake_ack,"):
        raise Exception("Invalid handshake ack")
    decrypted_ack = rsa_decrypt(ack[len(b"handshake_ack,"):].strip())
    if decrypted_ack != b"ack":
        raise Exception("Handshake ack invalid")
    logging.info("[Encryption] Handshake complete on sender side.")

def perform_handshake_receive(sock):
    """
    Receiver-side handshake:
      1. Receive "handshake_start" with peer's RSA public key.
      2. Send "handshake_response" with our RSA public key.
      3. Receive "handshake_key" and decrypt AES key.
      4. Send "handshake_ack" encrypted with peer's RSA public key.
    """
    # Step 1: Receive handshake_start
    data = sock.recv(4096)
    if not data.startswith(b"handshake_start,"):
        raise Exception("Invalid handshake start")
    peer_pubkey_pem = data[len(b"handshake_start,"):].strip()
    logging.info("[Encryption] Received handshake_start with peer RSA public key.")
    peer_public_key = serialization.load_pem_public_key(peer_pubkey_pem)

    # Step 2: Send our public key in handshake_response
    pubkey_pem = get_rsa_public_key_pem()
    sock.sendall(b"handshake_response," + pubkey_pem + b"\n")
    logging.info("[Encryption] Sent handshake_response with our RSA public key.")

    # Step 3: Receive handshake_key and decrypt AES key
    data = sock.recv(4096)
    if not data.startswith(b"handshake_key,"):
        raise Exception("Invalid handshake key")
    encrypted_aes = data[len(b"handshake_key,"):].strip()
    global AES_KEY
    AES_KEY = rsa_decrypt(encrypted_aes)
    logging.info("[Encryption] Received and decrypted AES key.")

    # Step 4: Send handshake_ack encrypted with peer's public key
    ack_encrypted = rsa_encrypt(b"ack", peer_public_key)
    sock.sendall(b"handshake_ack," + ack_encrypted + b"\n")
    logging.info("[Encryption] Sent handshake_ack. Handshake complete on receiver side.")

# --- Modified encryption functions for communication ---
def encrypt_message(message):
    """
    Encrypt message with AES encryption (after handshake).
    The original text message format (e.g. CSV) is maintained.
    """
    if AES_KEY is None:
        logging.error("[Encryption] AES key not established; message not encrypted.")
        return message.encode()
    logging.info(f"[Encryption] Encrypting message: {message}")
    encrypted = aes_encrypt(message.encode())
    return encrypted

def decrypt_message(message):
    """
    Decrypt AES encrypted message.
    """
    if AES_KEY is None:
        logging.error("[Encryption] AES key not established; cannot decrypt.")
        return message.decode()
    decrypted = aes_decrypt(message)
    logging.info(f"[Encryption] Decrypted message: {decrypted.decode()}")
    return decrypted.decode()

# Generate RSA keys at module load time
generate_rsa_keys()

