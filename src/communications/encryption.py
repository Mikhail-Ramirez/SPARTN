# communications/encryption.py
import socket
import logging
import os
import json
from Crypto.Cipher import AES, PKCS1_OAEP
from Crypto.PublicKey import RSA
from Crypto.Random import get_random_bytes

# def encrypt_message(message):
#     logging.info(f"[Dummy] encrypt_message called with message: {message}")
#     # Return a dummy encrypted message.
#     return f"encrypted:{message}"

# def decrypt_message(message):
#     logging.info(f"[Dummy] decrypt_message called with message: {message}")
#     # For simplicity, just remove the "encrypted:" prefix if it exists.
#     if message.startswith("encrypted:"):
#         return message[len("encrypted:"):]
#     return message

# RSA Key Generation
RSA_KEY = RSA.generate(2048)
PRIVATE_KEY = RSA_KEY.export_key()
PUBLIC_KEY = RSA_KEY.publickey().export_key()
AES_KEY = None  # Will be set after key exchange

# AES Encryption

def generate_aes_key():
    """Generate a random AES-128 key."""
    return get_random_bytes(16)

def encrypt_message(message):
    """Encrypts a message using AES-128-GCM."""
    global AES_KEY
    if AES_KEY is None:
        raise ValueError("AES key has not been established.")
    
    cipher = AES.new(AES_KEY, AES.MODE_GCM)
    ciphertext, tag = cipher.encrypt_and_digest(message.encode())
    return json.dumps({
        'nonce': cipher.nonce.hex(),
        'ciphertext': ciphertext.hex(),
        'tag': tag.hex()
    })

def decrypt_message(encrypted_data):
    """Decrypts a message using AES-128-GCM."""
    global AES_KEY
    if AES_KEY is None:
        raise ValueError("AES key has not been established.")
    
    data = json.loads(encrypted_data)
    cipher = AES.new(AES_KEY, AES.MODE_GCM, nonce=bytes.fromhex(data['nonce']))
    plaintext = cipher.decrypt_and_verify(bytes.fromhex(data['ciphertext']), bytes.fromhex(data['tag']))
    return plaintext.decode()

# RSA Key Exchange

def rsa_encrypt(message, pub_key):
    """Encrypts a message using RSA."""
    recipient_key = RSA.import_key(pub_key)
    cipher_rsa = PKCS1_OAEP.new(recipient_key)
    return cipher_rsa.encrypt(message)

def rsa_decrypt(encrypted_message):
    """Decrypts a message using RSA."""
    private_key = RSA.import_key(PRIVATE_KEY)
    cipher_rsa = PKCS1_OAEP.new(private_key)
    return cipher_rsa.decrypt(encrypted_message)

# Secure Communication Setup

def exchange_keys_with_tablet(sock):
    """Handles RSA-based key exchange between the tower and the tablet."""
    global AES_KEY
    sock.sendall(PUBLIC_KEY)  # Send RSA public key
    encrypted_aes_key = sock.recv(256)  # Receive AES key encrypted with RSA
    AES_KEY = rsa_decrypt(encrypted_aes_key)  # Decrypt AES key
    logging.info("[Key Exchange] AES Key successfully received and set.")

def send_encrypted_message(sock, message):
    """Sends an encrypted message over a socket."""
    encrypted = encrypt_message(message)
    sock.sendall(encrypted.encode())

def receive_encrypted_message(sock):
    """Receives and decrypts a message over a socket."""
    data = sock.recv(1024)
    return decrypt_message(data.decode())
