(ns collatz.baseline.core)

(defn collatz [n]
  (if (even? n)
    (/ n 2)
    (inc (* 3 n))))

(def collatz-seq-len
  (memoize
   (fn [n]
     (if (= n 1)
       1
       (inc (collatz-seq-len (collatz n)))))))

(prn (apply max-key collatz-seq-len (range 1 1e7)))
